static int shmem_removexattr(struct dentry *dentry, const char *name)
{
	struct shmem_inode_info *info = SHMEM_I(dentry->d_inode);
	int err;

	 
	if (!strncmp(name, XATTR_SYSTEM_PREFIX, XATTR_SYSTEM_PREFIX_LEN))
		return generic_removexattr(dentry, name);

	err = shmem_xattr_validate(name);
	if (err)
		return err;

	return simple_xattr_remove(&info->xattrs, name);
}
