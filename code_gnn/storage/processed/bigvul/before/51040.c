static bool path_connected(const struct path *path)
{
	struct vfsmount *mnt = path->mnt;

	 
	if (mnt->mnt_root == mnt->mnt_sb->s_root)
		return true;

	return is_subdir(path->dentry, mnt->mnt_root);
}
