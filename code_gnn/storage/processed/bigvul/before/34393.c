static noinline int submit_compressed_extents(struct inode *inode,
					      struct async_cow *async_cow)
{
	struct async_extent *async_extent;
	u64 alloc_hint = 0;
	struct btrfs_trans_handle *trans;
	struct btrfs_key ins;
	struct extent_map *em;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct extent_map_tree *em_tree = &BTRFS_I(inode)->extent_tree;
	struct extent_io_tree *io_tree;
	int ret = 0;

	if (list_empty(&async_cow->extents))
		return 0;


	while (!list_empty(&async_cow->extents)) {
		async_extent = list_entry(async_cow->extents.next,
					  struct async_extent, list);
		list_del(&async_extent->list);

		io_tree = &BTRFS_I(inode)->io_tree;

retry:
		 
		if (!async_extent->pages) {
			int page_started = 0;
			unsigned long nr_written = 0;

			lock_extent(io_tree, async_extent->start,
					 async_extent->start +
					 async_extent->ram_size - 1);

			 
			ret = cow_file_range(inode, async_cow->locked_page,
					     async_extent->start,
					     async_extent->start +
					     async_extent->ram_size - 1,
					     &page_started, &nr_written, 0);

			 

			 
			if (!page_started && !ret)
				extent_write_locked_range(io_tree,
						  inode, async_extent->start,
						  async_extent->start +
						  async_extent->ram_size - 1,
						  btrfs_get_extent,
						  WB_SYNC_ALL);
			kfree(async_extent);
			cond_resched();
			continue;
		}

		lock_extent(io_tree, async_extent->start,
			    async_extent->start + async_extent->ram_size - 1);

		trans = btrfs_join_transaction(root);
		if (IS_ERR(trans)) {
			ret = PTR_ERR(trans);
		} else {
			trans->block_rsv = &root->fs_info->delalloc_block_rsv;
			ret = btrfs_reserve_extent(trans, root,
					   async_extent->compressed_size,
					   async_extent->compressed_size,
					   0, alloc_hint, &ins, 1);
			if (ret && ret != -ENOSPC)
				btrfs_abort_transaction(trans, root, ret);
			btrfs_end_transaction(trans, root);
		}

		if (ret) {
			int i;
			for (i = 0; i < async_extent->nr_pages; i++) {
				WARN_ON(async_extent->pages[i]->mapping);
				page_cache_release(async_extent->pages[i]);
			}
			kfree(async_extent->pages);
			async_extent->nr_pages = 0;
			async_extent->pages = NULL;
			unlock_extent(io_tree, async_extent->start,
				      async_extent->start +
				      async_extent->ram_size - 1);
			if (ret == -ENOSPC)
				goto retry;
			goto out_free;  
		}

		 
		btrfs_drop_extent_cache(inode, async_extent->start,
					async_extent->start +
					async_extent->ram_size - 1, 0);

		em = alloc_extent_map();
		BUG_ON(!em);  
		em->start = async_extent->start;
		em->len = async_extent->ram_size;
		em->orig_start = em->start;

		em->block_start = ins.objectid;
		em->block_len = ins.offset;
		em->orig_block_len = ins.offset;
		em->bdev = root->fs_info->fs_devices->latest_bdev;
		em->compress_type = async_extent->compress_type;
		set_bit(EXTENT_FLAG_PINNED, &em->flags);
		set_bit(EXTENT_FLAG_COMPRESSED, &em->flags);
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
			btrfs_drop_extent_cache(inode, async_extent->start,
						async_extent->start +
						async_extent->ram_size - 1, 0);
		}

		ret = btrfs_add_ordered_extent_compress(inode,
						async_extent->start,
						ins.objectid,
						async_extent->ram_size,
						ins.offset,
						BTRFS_ORDERED_COMPRESSED,
						async_extent->compress_type);
		BUG_ON(ret);  

		 
		extent_clear_unlock_delalloc(inode,
				&BTRFS_I(inode)->io_tree,
				async_extent->start,
				async_extent->start +
				async_extent->ram_size - 1,
				NULL, EXTENT_CLEAR_UNLOCK_PAGE |
				EXTENT_CLEAR_UNLOCK |
				EXTENT_CLEAR_DELALLOC |
				EXTENT_CLEAR_DIRTY | EXTENT_SET_WRITEBACK);

		ret = btrfs_submit_compressed_write(inode,
				    async_extent->start,
				    async_extent->ram_size,
				    ins.objectid,
				    ins.offset, async_extent->pages,
				    async_extent->nr_pages);

		BUG_ON(ret);  
		alloc_hint = ins.objectid + ins.offset;
		kfree(async_extent);
		cond_resched();
	}
	ret = 0;
out:
	return ret;
out_free:
	kfree(async_extent);
	goto out;
}