static noinline int join_transaction(struct btrfs_root *root, int type)
{
	struct btrfs_transaction *cur_trans;
	struct btrfs_fs_info *fs_info = root->fs_info;

	spin_lock(&fs_info->trans_lock);
loop:
	 
	if (fs_info->fs_state & BTRFS_SUPER_FLAG_ERROR) {
		spin_unlock(&fs_info->trans_lock);
		return -EROFS;
	}

	if (fs_info->trans_no_join) {
		 
		if (type != TRANS_JOIN_NOLOCK) {
			spin_unlock(&fs_info->trans_lock);
			return -EBUSY;
		}
	}

	cur_trans = fs_info->running_transaction;
	if (cur_trans) {
		if (cur_trans->aborted) {
			spin_unlock(&fs_info->trans_lock);
			return cur_trans->aborted;
		}
		atomic_inc(&cur_trans->use_count);
		atomic_inc(&cur_trans->num_writers);
		cur_trans->num_joined++;
		spin_unlock(&fs_info->trans_lock);
		return 0;
	}
	spin_unlock(&fs_info->trans_lock);

	 
	if (type == TRANS_ATTACH)
		return -ENOENT;

	cur_trans = kmem_cache_alloc(btrfs_transaction_cachep, GFP_NOFS);
	if (!cur_trans)
		return -ENOMEM;

	spin_lock(&fs_info->trans_lock);
	if (fs_info->running_transaction) {
		 
		kmem_cache_free(btrfs_transaction_cachep, cur_trans);
		cur_trans = fs_info->running_transaction;
		goto loop;
	} else if (fs_info->fs_state & BTRFS_SUPER_FLAG_ERROR) {
		spin_unlock(&fs_info->trans_lock);
		kmem_cache_free(btrfs_transaction_cachep, cur_trans);
		return -EROFS;
	}

	atomic_set(&cur_trans->num_writers, 1);
	cur_trans->num_joined = 0;
	init_waitqueue_head(&cur_trans->writer_wait);
	init_waitqueue_head(&cur_trans->commit_wait);
	cur_trans->in_commit = 0;
	cur_trans->blocked = 0;
	 
	atomic_set(&cur_trans->use_count, 2);
	cur_trans->commit_done = 0;
	cur_trans->start_time = get_seconds();

	cur_trans->delayed_refs.root = RB_ROOT;
	cur_trans->delayed_refs.num_entries = 0;
	cur_trans->delayed_refs.num_heads_ready = 0;
	cur_trans->delayed_refs.num_heads = 0;
	cur_trans->delayed_refs.flushing = 0;
	cur_trans->delayed_refs.run_delayed_start = 0;

	 
	smp_mb();
	if (!list_empty(&fs_info->tree_mod_seq_list))
		WARN(1, KERN_ERR "btrfs: tree_mod_seq_list not empty when "
			"creating a fresh transaction\n");
	if (!RB_EMPTY_ROOT(&fs_info->tree_mod_log))
		WARN(1, KERN_ERR "btrfs: tree_mod_log rb tree not empty when "
			"creating a fresh transaction\n");
	atomic_set(&fs_info->tree_mod_seq, 0);

	spin_lock_init(&cur_trans->commit_lock);
	spin_lock_init(&cur_trans->delayed_refs.lock);

	INIT_LIST_HEAD(&cur_trans->pending_snapshots);
	list_add_tail(&cur_trans->list, &fs_info->trans_list);
	extent_io_tree_init(&cur_trans->dirty_pages,
			     fs_info->btree_inode->i_mapping);
	fs_info->generation++;
	cur_trans->transid = fs_info->generation;
	fs_info->running_transaction = cur_trans;
	cur_trans->aborted = 0;
	spin_unlock(&fs_info->trans_lock);

	return 0;
}