void kern_unmount(struct vfsmount *mnt)
{
	 
	if (!IS_ERR_OR_NULL(mnt)) {
		real_mount(mnt)->mnt_ns = NULL;
		synchronize_rcu();	 
		mntput(mnt);
	}
}
