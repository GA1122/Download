struct posix_acl *btrfs_get_acl(struct inode *inode, int type)
{
	int size;
	const char *name;
	char *value = NULL;
	struct posix_acl *acl;

	switch (type) {
	case ACL_TYPE_ACCESS:
		name = XATTR_NAME_POSIX_ACL_ACCESS;
		break;
	case ACL_TYPE_DEFAULT:
		name = XATTR_NAME_POSIX_ACL_DEFAULT;
		break;
	default:
		BUG();
	}

	size = __btrfs_getxattr(inode, name, "", 0);
	if (size > 0) {
		value = kzalloc(size, GFP_KERNEL);
		if (!value)
			return ERR_PTR(-ENOMEM);
		size = __btrfs_getxattr(inode, name, value, size);
	}
	if (size > 0) {
		acl = posix_acl_from_xattr(&init_user_ns, value, size);
	} else if (size == -ERANGE || size == -ENODATA || size == 0) {
		acl = NULL;
	} else {
		acl = ERR_PTR(-EIO);
	}
	kfree(value);

	return acl;
}
