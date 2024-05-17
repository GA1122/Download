int btrfs_commit_transaction(struct btrfs_trans_handle *trans,
			     struct btrfs_root *root)
{
	unsigned long joined = 0;
	struct btrfs_transaction *cur_trans = trans->transaction;
	struct btrfs_transaction *prev_trans = NULL;
	DEFINE_WAIT(wait);
	int ret;
	int should_grow = 0;
	unsigned long now = get_seconds();

	ret = btrfs_run_ordered_operations(root, 0);
	if (ret) {
		btrfs_abort_transaction(trans, root, ret);
		goto cleanup_transaction;
	}

	if (cur_trans->aborted) {
		ret = cur_trans->aborted;
		goto cleanup_transaction;
	}

	 
	ret = btrfs_run_delayed_refs(trans, root, 0);
	if (ret)
		goto cleanup_transaction;

	btrfs_trans_release_metadata(trans, root);
	trans->block_rsv = NULL;

	cur_trans = trans->transaction;

	 
	cur_trans->delayed_refs.flushing = 1;

	if (!list_empty(&trans->new_bgs))
		btrfs_create_pending_block_groups(trans, root);

	ret = btrfs_run_delayed_refs(trans, root, 0);
	if (ret)
		goto cleanup_transaction;

	spin_lock(&cur_trans->commit_lock);
	if (cur_trans->in_commit) {
		spin_unlock(&cur_trans->commit_lock);
		atomic_inc(&cur_trans->use_count);
		ret = btrfs_end_transaction(trans, root);

		wait_for_commit(root, cur_trans);

		put_transaction(cur_trans);

		return ret;
	}

	trans->transaction->in_commit = 1;
	trans->transaction->blocked = 1;
	spin_unlock(&cur_trans->commit_lock);
	wake_up(&root->fs_info->transaction_blocked_wait);

	spin_lock(&root->fs_info->trans_lock);
	if (cur_trans->list.prev != &root->fs_info->trans_list) {
		prev_trans = list_entry(cur_trans->list.prev,
					struct btrfs_transaction, list);
		if (!prev_trans->commit_done) {
			atomic_inc(&prev_trans->use_count);
			spin_unlock(&root->fs_info->trans_lock);

			wait_for_commit(root, prev_trans);

			put_transaction(prev_trans);
		} else {
			spin_unlock(&root->fs_info->trans_lock);
		}
	} else {
		spin_unlock(&root->fs_info->trans_lock);
	}

	if (!btrfs_test_opt(root, SSD) &&
	    (now < cur_trans->start_time || now - cur_trans->start_time < 1))
		should_grow = 1;

	do {
		joined = cur_trans->num_joined;

		WARN_ON(cur_trans != trans->transaction);

		ret = btrfs_flush_all_pending_stuffs(trans, root);
		if (ret)
			goto cleanup_transaction;

		prepare_to_wait(&cur_trans->writer_wait, &wait,
				TASK_UNINTERRUPTIBLE);

		if (atomic_read(&cur_trans->num_writers) > 1)
			schedule_timeout(MAX_SCHEDULE_TIMEOUT);
		else if (should_grow)
			schedule_timeout(1);

		finish_wait(&cur_trans->writer_wait, &wait);
	} while (atomic_read(&cur_trans->num_writers) > 1 ||
		 (should_grow && cur_trans->num_joined != joined));

	ret = btrfs_flush_all_pending_stuffs(trans, root);
	if (ret)
		goto cleanup_transaction;

	 
	spin_lock(&root->fs_info->trans_lock);
	root->fs_info->trans_no_join = 1;
	spin_unlock(&root->fs_info->trans_lock);
	wait_event(cur_trans->writer_wait,
		   atomic_read(&cur_trans->num_writers) == 1);

	 
	mutex_lock(&root->fs_info->reloc_mutex);

	 
	ret = create_pending_snapshots(trans, root->fs_info);
	if (ret) {
		mutex_unlock(&root->fs_info->reloc_mutex);
		goto cleanup_transaction;
	}

	 
	ret = btrfs_run_delayed_items(trans, root);
	if (ret) {
		mutex_unlock(&root->fs_info->reloc_mutex);
		goto cleanup_transaction;
	}

	ret = btrfs_run_delayed_refs(trans, root, (unsigned long)-1);
	if (ret) {
		mutex_unlock(&root->fs_info->reloc_mutex);
		goto cleanup_transaction;
	}

	 
	btrfs_assert_delayed_root_empty(root);

	WARN_ON(cur_trans != trans->transaction);

	btrfs_scrub_pause(root);
	 
	mutex_lock(&root->fs_info->tree_log_mutex);

	ret = commit_fs_roots(trans, root);
	if (ret) {
		mutex_unlock(&root->fs_info->tree_log_mutex);
		mutex_unlock(&root->fs_info->reloc_mutex);
		goto cleanup_transaction;
	}

	 
	btrfs_free_log_root_tree(trans, root->fs_info);

	ret = commit_cowonly_roots(trans, root);
	if (ret) {
		mutex_unlock(&root->fs_info->tree_log_mutex);
		mutex_unlock(&root->fs_info->reloc_mutex);
		goto cleanup_transaction;
	}

	btrfs_prepare_extent_commit(trans, root);

	cur_trans = root->fs_info->running_transaction;

	btrfs_set_root_node(&root->fs_info->tree_root->root_item,
			    root->fs_info->tree_root->node);
	switch_commit_root(root->fs_info->tree_root);

	btrfs_set_root_node(&root->fs_info->chunk_root->root_item,
			    root->fs_info->chunk_root->node);
	switch_commit_root(root->fs_info->chunk_root);

	assert_qgroups_uptodate(trans);
	update_super_roots(root);

	if (!root->fs_info->log_root_recovering) {
		btrfs_set_super_log_root(root->fs_info->super_copy, 0);
		btrfs_set_super_log_root_level(root->fs_info->super_copy, 0);
	}

	memcpy(root->fs_info->super_for_commit, root->fs_info->super_copy,
	       sizeof(*root->fs_info->super_copy));

	trans->transaction->blocked = 0;
	spin_lock(&root->fs_info->trans_lock);
	root->fs_info->running_transaction = NULL;
	root->fs_info->trans_no_join = 0;
	spin_unlock(&root->fs_info->trans_lock);
	mutex_unlock(&root->fs_info->reloc_mutex);

	wake_up(&root->fs_info->transaction_wait);

	ret = btrfs_write_and_wait_transaction(trans, root);
	if (ret) {
		btrfs_error(root->fs_info, ret,
			    "Error while writing out transaction.");
		mutex_unlock(&root->fs_info->tree_log_mutex);
		goto cleanup_transaction;
	}

	ret = write_ctree_super(trans, root, 0);
	if (ret) {
		mutex_unlock(&root->fs_info->tree_log_mutex);
		goto cleanup_transaction;
	}

	 
	mutex_unlock(&root->fs_info->tree_log_mutex);

	btrfs_finish_extent_commit(trans, root);

	cur_trans->commit_done = 1;

	root->fs_info->last_trans_committed = cur_trans->transid;

	wake_up(&cur_trans->commit_wait);

	spin_lock(&root->fs_info->trans_lock);
	list_del_init(&cur_trans->list);
	spin_unlock(&root->fs_info->trans_lock);

	put_transaction(cur_trans);
	put_transaction(cur_trans);

	if (trans->type < TRANS_JOIN_NOLOCK)
		sb_end_intwrite(root->fs_info->sb);

	trace_btrfs_transaction_commit(root);

	btrfs_scrub_continue(root);

	if (current->journal_info == trans)
		current->journal_info = NULL;

	kmem_cache_free(btrfs_trans_handle_cachep, trans);

	if (current != root->fs_info->transaction_kthread)
		btrfs_run_delayed_iputs(root);

	return ret;

cleanup_transaction:
	btrfs_trans_release_metadata(trans, root);
	trans->block_rsv = NULL;
	btrfs_printk(root->fs_info, "Skipping commit of aborted transaction.\n");
	if (current->journal_info == trans)
		current->journal_info = NULL;
	cleanup_transaction(trans, root, ret);

	return ret;
}