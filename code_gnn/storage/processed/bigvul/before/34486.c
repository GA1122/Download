static noinline int commit_cowonly_roots(struct btrfs_trans_handle *trans,
					 struct btrfs_root *root)
{
	struct btrfs_fs_info *fs_info = root->fs_info;
	struct list_head *next;
	struct extent_buffer *eb;
	int ret;

	ret = btrfs_run_delayed_refs(trans, root, (unsigned long)-1);
	if (ret)
		return ret;

	eb = btrfs_lock_root_node(fs_info->tree_root);
	ret = btrfs_cow_block(trans, fs_info->tree_root, eb, NULL,
			      0, &eb);
	btrfs_tree_unlock(eb);
	free_extent_buffer(eb);

	if (ret)
		return ret;

	ret = btrfs_run_delayed_refs(trans, root, (unsigned long)-1);
	if (ret)
		return ret;

	ret = btrfs_run_dev_stats(trans, root->fs_info);
	WARN_ON(ret);
	ret = btrfs_run_dev_replace(trans, root->fs_info);
	WARN_ON(ret);

	ret = btrfs_run_qgroups(trans, root->fs_info);
	BUG_ON(ret);

	 
	ret = btrfs_run_delayed_refs(trans, root, (unsigned long)-1);
	BUG_ON(ret);

	while (!list_empty(&fs_info->dirty_cowonly_roots)) {
		next = fs_info->dirty_cowonly_roots.next;
		list_del_init(next);
		root = list_entry(next, struct btrfs_root, dirty_list);

		ret = update_cowonly_root(trans, root);
		if (ret)
			return ret;
	}

	down_write(&fs_info->extent_commit_sem);
	switch_commit_root(fs_info->extent_root);
	up_write(&fs_info->extent_commit_sem);

	btrfs_after_dev_replace_commit(fs_info);

	return 0;
}