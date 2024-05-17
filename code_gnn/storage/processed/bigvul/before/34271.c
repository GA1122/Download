static noinline int __cow_file_range(struct btrfs_trans_handle *trans,
				     struct inode *inode,
				     struct btrfs_root *root,
				     struct page *locked_page,
				     u64 start, u64 end, int *page_started,
				     unsigned long *nr_written,
				     int unlock)
{
	u64 alloc_hint = 0;
	u64 num_bytes;
	unsigned long ram_size;
	u64 disk_num_bytes;
	u64 cur_alloc_size;
	u64 blocksize = root->sectorsize;
	struct btrfs_key ins;
	struct extent_map *em;
	struct extent_map_tree *em_tree = &BTRFS_I(inode)->extent_tree;
	int ret = 0;

	BUG_ON(btrfs_is_free_space_inode(inode));

	num_bytes = (end - start + blocksize) & ~(blocksize - 1);
	num_bytes = max(blocksize,  num_bytes);
	disk_num_bytes = num_bytes;

	 
	if (num_bytes < 64 * 1024 &&
	    (start > 0 || end + 1 < BTRFS_I(inode)->disk_i_size))
		btrfs_add_inode_defrag(trans, inode);

	if (start == 0) {
		 
		ret = cow_file_range_inline(trans, root, inode,
					    start, end, 0, 0, NULL);
		if (ret == 0) {
			extent_clear_unlock_delalloc(inode,
				     &BTRFS_I(inode)->io_tree,
				     start, end, NULL,
				     EXTENT_CLEAR_UNLOCK_PAGE |
				     EXTENT_CLEAR_UNLOCK |
				     EXTENT_CLEAR_DELALLOC |
				     EXTENT_CLEAR_DIRTY |
				     EXTENT_SET_WRITEBACK |
				     EXTENT_END_WRITEBACK);

			*nr_written = *nr_written +
			     (end - start + PAGE_CACHE_SIZE) / PAGE_CACHE_SIZE;
			*page_started = 1;
			goto out;
		} else if (ret < 0) {
			btrfs_abort_transaction(trans, root, ret);
			goto out_unlock;
		}
	}

	BUG_ON(disk_num_bytes >
	       btrfs_super_total_bytes(root->fs_info->super_copy));

	alloc_hint = get_extent_allocation_hint(inode, start, num_bytes);
	btrfs_drop_extent_cache(inode, start, start + num_bytes - 1, 0);

	while (disk_num_bytes > 0) {
		unsigned long op;

		cur_alloc_size = disk_num_bytes;
		ret = btrfs_reserve_extent(trans, root, cur_alloc_size,
					   root->sectorsize, 0, alloc_hint,
					   &ins, 1);
		if (ret < 0) {
			btrfs_abort_transaction(trans, root, ret);
			goto out_unlock;
		}

		em = alloc_extent_map();
		BUG_ON(!em);  
		em->start = start;
		em->orig_start = em->start;
		ram_size = ins.offset;
		em->len = ins.offset;

		em->block_start = ins.objectid;
		em->block_len = ins.offset;
		em->orig_block_len = ins.offset;
		em->bdev = root->fs_info->fs_devices->latest_bdev;
		set_bit(EXTENT_FLAG_PINNED, &em->flags);
		em->generation = -1;

		while (1) {
			write_lock(&em_tree->lock);
			ret = add_extent_mapping(em_tree, em);
			if (!ret)
				list_move(&em->list,
					  &em_tree->modified_extents);
			write_unlock(&em_tree->lock);
			if (ret != -EEXIST) {
				free_extent_map(em);
				break;
			}
			btrfs_drop_extent_cache(inode, start,
						start + ram_size - 1, 0);
		}

		cur_alloc_size = ins.offset;
		ret = btrfs_add_ordered_extent(inode, start, ins.objectid,
					       ram_size, cur_alloc_size, 0);
		BUG_ON(ret);  

		if (root->root_key.objectid ==
		    BTRFS_DATA_RELOC_TREE_OBJECTID) {
			ret = btrfs_reloc_clone_csums(inode, start,
						      cur_alloc_size);
			if (ret) {
				btrfs_abort_transaction(trans, root, ret);
				goto out_unlock;
			}
		}

		if (disk_num_bytes < cur_alloc_size)
			break;

		 
		op = unlock ? EXTENT_CLEAR_UNLOCK_PAGE : 0;
		op |= EXTENT_CLEAR_UNLOCK | EXTENT_CLEAR_DELALLOC |
			EXTENT_SET_PRIVATE2;

		extent_clear_unlock_delalloc(inode, &BTRFS_I(inode)->io_tree,
					     start, start + ram_size - 1,
					     locked_page, op);
		disk_num_bytes -= cur_alloc_size;
		num_bytes -= cur_alloc_size;
		alloc_hint = ins.objectid + ins.offset;
		start += cur_alloc_size;
	}
out:
	return ret;

out_unlock:
	extent_clear_unlock_delalloc(inode,
		     &BTRFS_I(inode)->io_tree,
		     start, end, locked_page,
		     EXTENT_CLEAR_UNLOCK_PAGE |
		     EXTENT_CLEAR_UNLOCK |
		     EXTENT_CLEAR_DELALLOC |
		     EXTENT_CLEAR_DIRTY |
		     EXTENT_SET_WRITEBACK |
		     EXTENT_END_WRITEBACK);

	goto out;
}