static int do_add_mount(struct mount *newmnt, struct path *path, int mnt_flags)
{
	int err;

	mnt_flags &= ~(MNT_SHARED | MNT_WRITE_HOLD | MNT_INTERNAL);

	err = lock_mount(path);
	if (err)
		return err;

	err = -EINVAL;
	if (unlikely(!check_mnt(real_mount(path->mnt)))) {
		 
		if (!(mnt_flags & MNT_SHRINKABLE))
			goto unlock;
		 
		if (!real_mount(path->mnt)->mnt_ns)
			goto unlock;
	}

	 
	err = -EBUSY;
	if (path->mnt->mnt_sb == newmnt->mnt.mnt_sb &&
	    path->mnt->mnt_root == path->dentry)
		goto unlock;

	err = -EINVAL;
	if (S_ISLNK(newmnt->mnt.mnt_root->d_inode->i_mode))
		goto unlock;

	newmnt->mnt.mnt_flags = mnt_flags;
	err = graft_tree(newmnt, path);

unlock:
	unlock_mount(path);
	return err;
}