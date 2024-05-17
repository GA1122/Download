static int shmem_setxattr(struct dentry *dentry, const char *name,
			  const void *value, size_t size, int flags)
{
	struct shmem_inode_info *info = SHMEM_I(dentry->d_inode);
	int err;

	 
	if (!strncmp(name, XATTR_SYSTEM_PREFIX, XATTR_SYSTEM_PREFIX_LEN))
		return generic_setxattr(dentry, name, value, size, flags);

	err = shmem_xattr_validate(name);
	if (err)
		return err;

	return simple_xattr_set(&info->xattrs, name, value, size, flags);
}