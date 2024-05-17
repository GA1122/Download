static bool disconnect_mount(struct mount *mnt, enum umount_tree_flags how)
{
	 
	if (how & UMOUNT_SYNC)
		return true;

	 
	if (!mnt_has_parent(mnt))
		return true;

	 
	if (!(mnt->mnt_parent->mnt.mnt_flags & MNT_UMOUNT))
		return true;

	 
	if (how & UMOUNT_CONNECTED)
		return false;

	 
	if (IS_MNT_LOCKED(mnt))
		return false;

	 
	return true;
}
