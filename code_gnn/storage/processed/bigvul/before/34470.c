static int btrfs_flush_all_pending_stuffs(struct btrfs_trans_handle *trans,
					  struct btrfs_root *root)
{
	int flush_on_commit = btrfs_test_opt(root, FLUSHONCOMMIT);
	int snap_pending = 0;
	int ret;

	if (!flush_on_commit) {
		spin_lock(&root->fs_info->trans_lock);
		if (!list_empty(&trans->transaction->pending_snapshots))
			snap_pending = 1;
		spin_unlock(&root->fs_info->trans_lock);
	}

	if (flush_on_commit || snap_pending) {
		btrfs_start_delalloc_inodes(root, 1);
		btrfs_wait_ordered_extents(root, 1);
	}

	ret = btrfs_run_delayed_items(trans, root);
	if (ret)
		return ret;

	 
	btrfs_delayed_refs_qgroup_accounting(trans, root->fs_info);

	 
	btrfs_run_ordered_operations(root, 1);

	return 0;
}
