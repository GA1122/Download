static int relatime_need_update(struct vfsmount *mnt, struct inode *inode,
			     struct timespec now)
{

	if (!(mnt->mnt_flags & MNT_RELATIME))
		return 1;
	 
	if (timespec_compare(&inode->i_mtime, &inode->i_atime) >= 0)
		return 1;
	 
	if (timespec_compare(&inode->i_ctime, &inode->i_atime) >= 0)
		return 1;

	 
	if ((long)(now.tv_sec - inode->i_atime.tv_sec) >= 24*60*60)
		return 1;
	 
	return 0;
}
