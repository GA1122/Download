static int mnt_make_readonly(struct mount *mnt)
{
	int ret = 0;

	br_write_lock(&vfsmount_lock);
	mnt->mnt.mnt_flags |= MNT_WRITE_HOLD;
	 
	smp_mb();

	 
	if (mnt_get_writers(mnt) > 0)
		ret = -EBUSY;
	else
		mnt->mnt.mnt_flags |= MNT_READONLY;
	 
	smp_wmb();
	mnt->mnt.mnt_flags &= ~MNT_WRITE_HOLD;
	br_write_unlock(&vfsmount_lock);
	return ret;
}
