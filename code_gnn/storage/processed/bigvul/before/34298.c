void btrfs_evict_inode(struct inode *inode)
{
	struct btrfs_trans_handle *trans;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct btrfs_block_rsv *rsv, *global_rsv;
	u64 min_size = btrfs_calc_trunc_metadata_size(root, 1);
	int ret;

	trace_btrfs_inode_evict(inode);

	truncate_inode_pages(&inode->i_data, 0);
	if (inode->i_nlink && (btrfs_root_refs(&root->root_item) != 0 ||
			       btrfs_is_free_space_inode(inode)))
		goto no_delete;

	if (is_bad_inode(inode)) {
		btrfs_orphan_del(NULL, inode);
		goto no_delete;
	}
	 
	btrfs_wait_ordered_range(inode, 0, (u64)-1);

	if (root->fs_info->log_root_recovering) {
		BUG_ON(test_bit(BTRFS_INODE_HAS_ORPHAN_ITEM,
				 &BTRFS_I(inode)->runtime_flags));
		goto no_delete;
	}

	if (inode->i_nlink > 0) {
		BUG_ON(btrfs_root_refs(&root->root_item) != 0);
		goto no_delete;
	}

	rsv = btrfs_alloc_block_rsv(root, BTRFS_BLOCK_RSV_TEMP);
	if (!rsv) {
		btrfs_orphan_del(NULL, inode);
		goto no_delete;
	}
	rsv->size = min_size;
	rsv->failfast = 1;
	global_rsv = &root->fs_info->global_block_rsv;

	btrfs_i_size_write(inode, 0);

	 
	while (1) {
		ret = btrfs_block_rsv_refill(root, rsv, min_size,
					     BTRFS_RESERVE_FLUSH_LIMIT);

		 
		if (ret)
			ret = btrfs_block_rsv_migrate(global_rsv, rsv, min_size);

		if (ret) {
			printk(KERN_WARNING "Could not get space for a "
			       "delete, will truncate on mount %d\n", ret);
			btrfs_orphan_del(NULL, inode);
			btrfs_free_block_rsv(root, rsv);
			goto no_delete;
		}

		trans = btrfs_start_transaction_lflush(root, 1);
		if (IS_ERR(trans)) {
			btrfs_orphan_del(NULL, inode);
			btrfs_free_block_rsv(root, rsv);
			goto no_delete;
		}

		trans->block_rsv = rsv;

		ret = btrfs_truncate_inode_items(trans, root, inode, 0, 0);
		if (ret != -ENOSPC)
			break;

		trans->block_rsv = &root->fs_info->trans_block_rsv;
		ret = btrfs_update_inode(trans, root, inode);
		BUG_ON(ret);

		btrfs_end_transaction(trans, root);
		trans = NULL;
		btrfs_btree_balance_dirty(root);
	}

	btrfs_free_block_rsv(root, rsv);

	if (ret == 0) {
		trans->block_rsv = root->orphan_block_rsv;
		ret = btrfs_orphan_del(trans, inode);
		BUG_ON(ret);
	}

	trans->block_rsv = &root->fs_info->trans_block_rsv;
	if (!(root == root->fs_info->tree_root ||
	      root->root_key.objectid == BTRFS_TREE_RELOC_OBJECTID))
		btrfs_return_ino(root, btrfs_ino(inode));

	btrfs_end_transaction(trans, root);
	btrfs_btree_balance_dirty(root);
no_delete:
	clear_inode(inode);
	return;
}