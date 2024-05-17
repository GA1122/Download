int btrfs_setxattr(struct dentry *dentry, const char *name, const void *value,
		   size_t size, int flags)
{
	struct btrfs_root *root = BTRFS_I(dentry->d_inode)->root;

	 
	if (btrfs_root_readonly(root))
		return -EROFS;

	 
	if (!strncmp(name, XATTR_SYSTEM_PREFIX, XATTR_SYSTEM_PREFIX_LEN))
		return generic_setxattr(dentry, name, value, size, flags);

	if (!btrfs_is_valid_xattr(name))
		return -EOPNOTSUPP;

	if (!strncmp(name, XATTR_BTRFS_PREFIX, XATTR_BTRFS_PREFIX_LEN))
		return btrfs_set_prop(dentry->d_inode, name,
				      value, size, flags);

	if (size == 0)
		value = "";   

	return __btrfs_setxattr(NULL, dentry->d_inode, name, value, size,
				flags);
}
