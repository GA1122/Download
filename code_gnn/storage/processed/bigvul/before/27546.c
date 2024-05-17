static int inotify_new_watch(struct fsnotify_group *group,
			     struct inode *inode,
			     u32 arg)
{
	struct inotify_inode_mark *tmp_i_mark;
	__u32 mask;
	int ret;
	struct idr *idr = &group->inotify_data.idr;
	spinlock_t *idr_lock = &group->inotify_data.idr_lock;

	 
	mask = inotify_arg_to_mask(arg);
	if (unlikely(!(mask & IN_ALL_EVENTS)))
		return -EINVAL;

	tmp_i_mark = kmem_cache_alloc(inotify_inode_mark_cachep, GFP_KERNEL);
	if (unlikely(!tmp_i_mark))
		return -ENOMEM;

	fsnotify_init_mark(&tmp_i_mark->fsn_mark, inotify_free_mark);
	tmp_i_mark->fsn_mark.mask = mask;
	tmp_i_mark->wd = -1;

	ret = -ENOSPC;
	if (atomic_read(&group->inotify_data.user->inotify_watches) >= inotify_max_user_watches)
		goto out_err;

	ret = inotify_add_to_idr(idr, idr_lock, &group->inotify_data.last_wd,
				 tmp_i_mark);
	if (ret)
		goto out_err;

	 
	ret = fsnotify_add_mark(&tmp_i_mark->fsn_mark, group, inode, NULL, 0);
	if (ret) {
		 
		inotify_remove_from_idr(group, tmp_i_mark);
		goto out_err;
	}

	 
	atomic_inc(&group->inotify_data.user->inotify_watches);

	 
	ret = tmp_i_mark->wd;

out_err:
	 
	fsnotify_put_mark(&tmp_i_mark->fsn_mark);

	return ret;
}
