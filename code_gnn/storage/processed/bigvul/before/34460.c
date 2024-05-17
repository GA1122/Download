static int __btrfs_end_transaction(struct btrfs_trans_handle *trans,
			  struct btrfs_root *root, int throttle)
{
	struct btrfs_transaction *cur_trans = trans->transaction;
	struct btrfs_fs_info *info = root->fs_info;
	int count = 0;
	int lock = (trans->type != TRANS_JOIN_NOLOCK);
	int err = 0;

	if (--trans->use_count) {
		trans->block_rsv = trans->orig_rsv;
		return 0;
	}

	 
	err = btrfs_delayed_refs_qgroup_accounting(trans, info);

	btrfs_trans_release_metadata(trans, root);
	trans->block_rsv = NULL;
	 
	WARN_ON(trans->root != root);

	if (trans->qgroup_reserved) {
		btrfs_qgroup_free(root, trans->qgroup_reserved);
		trans->qgroup_reserved = 0;
	}

	if (!list_empty(&trans->new_bgs))
		btrfs_create_pending_block_groups(trans, root);

	while (count < 2) {
		unsigned long cur = trans->delayed_ref_updates;
		trans->delayed_ref_updates = 0;
		if (cur &&
		    trans->transaction->delayed_refs.num_heads_ready > 64) {
			trans->delayed_ref_updates = 0;
			btrfs_run_delayed_refs(trans, root, cur);
		} else {
			break;
		}
		count++;
	}
	btrfs_trans_release_metadata(trans, root);
	trans->block_rsv = NULL;

	if (!list_empty(&trans->new_bgs))
		btrfs_create_pending_block_groups(trans, root);

	if (lock && !atomic_read(&root->fs_info->open_ioctl_trans) &&
	    should_end_transaction(trans, root)) {
		trans->transaction->blocked = 1;
		smp_wmb();
	}

	if (lock && cur_trans->blocked && !cur_trans->in_commit) {
		if (throttle) {
			 
			trans->use_count++;
			return btrfs_commit_transaction(trans, root);
		} else {
			wake_up_process(info->transaction_kthread);
		}
	}

	if (trans->type < TRANS_JOIN_NOLOCK)
		sb_end_intwrite(root->fs_info->sb);

	WARN_ON(cur_trans != info->running_transaction);
	WARN_ON(atomic_read(&cur_trans->num_writers) < 1);
	atomic_dec(&cur_trans->num_writers);

	smp_mb();
	if (waitqueue_active(&cur_trans->writer_wait))
		wake_up(&cur_trans->writer_wait);
	put_transaction(cur_trans);

	if (current->journal_info == trans)
		current->journal_info = NULL;

	if (throttle)
		btrfs_run_delayed_iputs(root);

	if (trans->aborted ||
	    root->fs_info->fs_state & BTRFS_SUPER_FLAG_ERROR) {
		err = -EIO;
	}
	assert_qgroups_uptodate(trans);

	memset(trans, 0, sizeof(*trans));
	kmem_cache_free(btrfs_trans_handle_cachep, trans);
	return err;
}