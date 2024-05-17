static int mnt_make_readonly(struct mount *mnt)
{
	int ret = 0;

	lock_mount_hash();
	mnt->mnt.mnt_flags |= MNT_WRITE_HOLD;
	 
	smp_mb();

	 
	if (mnt_get_writers(mnt) > 0)
		ret = -EBUSY;
	else
		mnt->mnt.mnt_flags |= MNT_READONLY;
	 
	smp_wmb();
	mnt->mnt.mnt_flags &= ~MNT_WRITE_HOLD;
	unlock_mount_hash();
	return ret;
}
