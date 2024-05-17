static void umount_mnt(struct mount *mnt)
{
	 
	mnt->mnt_ex_mountpoint = mnt->mnt_mountpoint;
	unhash_mnt(mnt);
}
