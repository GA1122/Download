struct extent_map *btrfs_get_extent(struct inode *inode, struct page *page,
				    size_t pg_offset, u64 start, u64 len,
				    int create)
{
	int ret;
	int err = 0;
	u64 bytenr;
	u64 extent_start = 0;
	u64 extent_end = 0;
	u64 objectid = btrfs_ino(inode);
	u32 found_type;
	struct btrfs_path *path = NULL;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct btrfs_file_extent_item *item;
	struct extent_buffer *leaf;
	struct btrfs_key found_key;
	struct extent_map *em = NULL;
	struct extent_map_tree *em_tree = &BTRFS_I(inode)->extent_tree;
	struct extent_io_tree *io_tree = &BTRFS_I(inode)->io_tree;
	struct btrfs_trans_handle *trans = NULL;
	int compress_type;

again:
	read_lock(&em_tree->lock);
	em = lookup_extent_mapping(em_tree, start, len);
	if (em)
		em->bdev = root->fs_info->fs_devices->latest_bdev;
	read_unlock(&em_tree->lock);

	if (em) {
		if (em->start > start || em->start + em->len <= start)
			free_extent_map(em);
		else if (em->block_start == EXTENT_MAP_INLINE && page)
			free_extent_map(em);
		else
			goto out;
	}
	em = alloc_extent_map();
	if (!em) {
		err = -ENOMEM;
		goto out;
	}
	em->bdev = root->fs_info->fs_devices->latest_bdev;
	em->start = EXTENT_MAP_HOLE;
	em->orig_start = EXTENT_MAP_HOLE;
	em->len = (u64)-1;
	em->block_len = (u64)-1;

	if (!path) {
		path = btrfs_alloc_path();
		if (!path) {
			err = -ENOMEM;
			goto out;
		}
		 
		path->reada = 1;
	}

	ret = btrfs_lookup_file_extent(trans, root, path,
				       objectid, start, trans != NULL);
	if (ret < 0) {
		err = ret;
		goto out;
	}

	if (ret != 0) {
		if (path->slots[0] == 0)
			goto not_found;
		path->slots[0]--;
	}

	leaf = path->nodes[0];
	item = btrfs_item_ptr(leaf, path->slots[0],
			      struct btrfs_file_extent_item);
	 
	btrfs_item_key_to_cpu(leaf, &found_key, path->slots[0]);
	found_type = btrfs_key_type(&found_key);
	if (found_key.objectid != objectid ||
	    found_type != BTRFS_EXTENT_DATA_KEY) {
		goto not_found;
	}

	found_type = btrfs_file_extent_type(leaf, item);
	extent_start = found_key.offset;
	compress_type = btrfs_file_extent_compression(leaf, item);
	if (found_type == BTRFS_FILE_EXTENT_REG ||
	    found_type == BTRFS_FILE_EXTENT_PREALLOC) {
		extent_end = extent_start +
		       btrfs_file_extent_num_bytes(leaf, item);
	} else if (found_type == BTRFS_FILE_EXTENT_INLINE) {
		size_t size;
		size = btrfs_file_extent_inline_len(leaf, item);
		extent_end = (extent_start + size + root->sectorsize - 1) &
			~((u64)root->sectorsize - 1);
	}

	if (start >= extent_end) {
		path->slots[0]++;
		if (path->slots[0] >= btrfs_header_nritems(leaf)) {
			ret = btrfs_next_leaf(root, path);
			if (ret < 0) {
				err = ret;
				goto out;
			}
			if (ret > 0)
				goto not_found;
			leaf = path->nodes[0];
		}
		btrfs_item_key_to_cpu(leaf, &found_key, path->slots[0]);
		if (found_key.objectid != objectid ||
		    found_key.type != BTRFS_EXTENT_DATA_KEY)
			goto not_found;
		if (start + len <= found_key.offset)
			goto not_found;
		em->start = start;
		em->orig_start = start;
		em->len = found_key.offset - start;
		goto not_found_em;
	}

	if (found_type == BTRFS_FILE_EXTENT_REG ||
	    found_type == BTRFS_FILE_EXTENT_PREALLOC) {
		em->start = extent_start;
		em->len = extent_end - extent_start;
		em->orig_start = extent_start -
				 btrfs_file_extent_offset(leaf, item);
		em->orig_block_len = btrfs_file_extent_disk_num_bytes(leaf,
								      item);
		bytenr = btrfs_file_extent_disk_bytenr(leaf, item);
		if (bytenr == 0) {
			em->block_start = EXTENT_MAP_HOLE;
			goto insert;
		}
		if (compress_type != BTRFS_COMPRESS_NONE) {
			set_bit(EXTENT_FLAG_COMPRESSED, &em->flags);
			em->compress_type = compress_type;
			em->block_start = bytenr;
			em->block_len = em->orig_block_len;
		} else {
			bytenr += btrfs_file_extent_offset(leaf, item);
			em->block_start = bytenr;
			em->block_len = em->len;
			if (found_type == BTRFS_FILE_EXTENT_PREALLOC)
				set_bit(EXTENT_FLAG_PREALLOC, &em->flags);
		}
		goto insert;
	} else if (found_type == BTRFS_FILE_EXTENT_INLINE) {
		unsigned long ptr;
		char *map;
		size_t size;
		size_t extent_offset;
		size_t copy_size;

		em->block_start = EXTENT_MAP_INLINE;
		if (!page || create) {
			em->start = extent_start;
			em->len = extent_end - extent_start;
			goto out;
		}

		size = btrfs_file_extent_inline_len(leaf, item);
		extent_offset = page_offset(page) + pg_offset - extent_start;
		copy_size = min_t(u64, PAGE_CACHE_SIZE - pg_offset,
				size - extent_offset);
		em->start = extent_start + extent_offset;
		em->len = (copy_size + root->sectorsize - 1) &
			~((u64)root->sectorsize - 1);
		em->orig_block_len = em->len;
		em->orig_start = em->start;
		if (compress_type) {
			set_bit(EXTENT_FLAG_COMPRESSED, &em->flags);
			em->compress_type = compress_type;
		}
		ptr = btrfs_file_extent_inline_start(item) + extent_offset;
		if (create == 0 && !PageUptodate(page)) {
			if (btrfs_file_extent_compression(leaf, item) !=
			    BTRFS_COMPRESS_NONE) {
				ret = uncompress_inline(path, inode, page,
							pg_offset,
							extent_offset, item);
				BUG_ON(ret);  
			} else {
				map = kmap(page);
				read_extent_buffer(leaf, map + pg_offset, ptr,
						   copy_size);
				if (pg_offset + copy_size < PAGE_CACHE_SIZE) {
					memset(map + pg_offset + copy_size, 0,
					       PAGE_CACHE_SIZE - pg_offset -
					       copy_size);
				}
				kunmap(page);
			}
			flush_dcache_page(page);
		} else if (create && PageUptodate(page)) {
			BUG();
			if (!trans) {
				kunmap(page);
				free_extent_map(em);
				em = NULL;

				btrfs_release_path(path);
				trans = btrfs_join_transaction(root);

				if (IS_ERR(trans))
					return ERR_CAST(trans);
				goto again;
			}
			map = kmap(page);
			write_extent_buffer(leaf, map + pg_offset, ptr,
					    copy_size);
			kunmap(page);
			btrfs_mark_buffer_dirty(leaf);
		}
		set_extent_uptodate(io_tree, em->start,
				    extent_map_end(em) - 1, NULL, GFP_NOFS);
		goto insert;
	} else {
		WARN(1, KERN_ERR "btrfs unknown found_type %d\n", found_type);
	}
not_found:
	em->start = start;
	em->orig_start = start;
	em->len = len;
not_found_em:
	em->block_start = EXTENT_MAP_HOLE;
	set_bit(EXTENT_FLAG_VACANCY, &em->flags);
insert:
	btrfs_release_path(path);
	if (em->start > start || extent_map_end(em) <= start) {
		printk(KERN_ERR "Btrfs: bad extent! em: [%llu %llu] passed "
		       "[%llu %llu]\n", (unsigned long long)em->start,
		       (unsigned long long)em->len,
		       (unsigned long long)start,
		       (unsigned long long)len);
		err = -EIO;
		goto out;
	}

	err = 0;
	write_lock(&em_tree->lock);
	ret = add_extent_mapping(em_tree, em);
	 
	if (ret == -EEXIST) {
		struct extent_map *existing;

		ret = 0;

		existing = lookup_extent_mapping(em_tree, start, len);
		if (existing && (existing->start > start ||
		    existing->start + existing->len <= start)) {
			free_extent_map(existing);
			existing = NULL;
		}
		if (!existing) {
			existing = lookup_extent_mapping(em_tree, em->start,
							 em->len);
			if (existing) {
				err = merge_extent_mapping(em_tree, existing,
							   em, start,
							   root->sectorsize);
				free_extent_map(existing);
				if (err) {
					free_extent_map(em);
					em = NULL;
				}
			} else {
				err = -EIO;
				free_extent_map(em);
				em = NULL;
			}
		} else {
			free_extent_map(em);
			em = existing;
			err = 0;
		}
	}
	write_unlock(&em_tree->lock);
out:

	if (em)
		trace_btrfs_get_extent(root, em);

	if (path)
		btrfs_free_path(path);
	if (trans) {
		ret = btrfs_end_transaction(trans, root);
		if (!err)
			err = ret;
	}
	if (err) {
		free_extent_map(em);
		return ERR_PTR(err);
	}
	BUG_ON(!em);  
	return em;
}