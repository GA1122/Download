static int inotify_update_watch(struct fsnotify_group *group, struct inode *inode, u32 arg)
{
	int ret = 0;

retry:
	 
	ret = inotify_update_existing_watch(group, inode, arg);
	 
	if (ret == -ENOENT)
		ret = inotify_new_watch(group, inode, arg);
	 
	if (ret == -EEXIST)
		goto retry;

	return ret;
}
