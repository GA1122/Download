struct posix_acl *hfsplus_get_posix_acl(struct inode *inode, int type)
{
	struct posix_acl *acl;
	char *xattr_name;
	char *value = NULL;
	ssize_t size;

	hfs_dbg(ACL_MOD, "[%s]: ino %lu\n", __func__, inode->i_ino);

	switch (type) {
	case ACL_TYPE_ACCESS:
		xattr_name = XATTR_NAME_POSIX_ACL_ACCESS;
		break;
	case ACL_TYPE_DEFAULT:
		xattr_name = XATTR_NAME_POSIX_ACL_DEFAULT;
		break;
	default:
		return ERR_PTR(-EINVAL);
	}

	size = __hfsplus_getxattr(inode, xattr_name, NULL, 0);

	if (size > 0) {
		value = (char *)hfsplus_alloc_attr_entry();
		if (unlikely(!value))
			return ERR_PTR(-ENOMEM);
		size = __hfsplus_getxattr(inode, xattr_name, value, size);
	}

	if (size > 0)
		acl = posix_acl_from_xattr(&init_user_ns, value, size);
	else if (size == -ENODATA)
		acl = NULL;
	else
		acl = ERR_PTR(size);

	hfsplus_destroy_attr_entry((hfsplus_attr_entry *)value);

	return acl;
}
