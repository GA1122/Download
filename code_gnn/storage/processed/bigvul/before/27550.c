static int inotify_update_existing_watch(struct fsnotify_group *group,
					 struct inode *inode,
					 u32 arg)
{
	struct fsnotify_mark *fsn_mark;
	struct inotify_inode_mark *i_mark;
	__u32 old_mask, new_mask;
	__u32 mask;
	int add = (arg & IN_MASK_ADD);
	int ret;

	 
	mask = inotify_arg_to_mask(arg);
	if (unlikely(!(mask & IN_ALL_EVENTS)))
		return -EINVAL;

	fsn_mark = fsnotify_find_inode_mark(group, inode);
	if (!fsn_mark)
		return -ENOENT;

	i_mark = container_of(fsn_mark, struct inotify_inode_mark, fsn_mark);

	spin_lock(&fsn_mark->lock);

	old_mask = fsn_mark->mask;
	if (add)
		fsnotify_set_mark_mask_locked(fsn_mark, (fsn_mark->mask | mask));
	else
		fsnotify_set_mark_mask_locked(fsn_mark, mask);
	new_mask = fsn_mark->mask;

	spin_unlock(&fsn_mark->lock);

	if (old_mask != new_mask) {
		 
		int dropped = (old_mask & ~new_mask);
		 
		int do_inode = (new_mask & ~inode->i_fsnotify_mask);

		 
		if (dropped || do_inode)
			fsnotify_recalc_inode_mask(inode);

	}

	 
	ret = i_mark->wd;

	 
	fsnotify_put_mark(fsn_mark);

	return ret;
}
