static int mnt_is_readonly(struct vfsmount *mnt)
{
	if (mnt->mnt_sb->s_readonly_remount)
		return 1;
	 
	smp_rmb();
	return __mnt_is_readonly(mnt);
}
