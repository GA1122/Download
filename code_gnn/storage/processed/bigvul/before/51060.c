static int ovl_dir_getattr(struct vfsmount *mnt, struct dentry *dentry,
			 struct kstat *stat)
{
	int err;
	enum ovl_path_type type;
	struct path realpath;

	type = ovl_path_real(dentry, &realpath);
	err = vfs_getattr(&realpath, stat);
	if (err)
		return err;

	stat->dev = dentry->d_sb->s_dev;
	stat->ino = dentry->d_inode->i_ino;

	 
	if (OVL_TYPE_MERGE(type))
		stat->nlink = 1;

	return 0;
}
