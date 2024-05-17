xfs_get_acl(struct inode *inode, int type)
{
	struct xfs_inode *ip = XFS_I(inode);
	struct posix_acl *acl = NULL;
	struct xfs_acl *xfs_acl;
	unsigned char *ea_name;
	int error;
	int len;

	trace_xfs_get_acl(ip);

	switch (type) {
	case ACL_TYPE_ACCESS:
		ea_name = SGI_ACL_FILE;
		break;
	case ACL_TYPE_DEFAULT:
		ea_name = SGI_ACL_DEFAULT;
		break;
	default:
		BUG();
	}

	 
	len = XFS_ACL_MAX_SIZE(ip->i_mount);
	xfs_acl = kmem_zalloc_large(len, KM_SLEEP);
	if (!xfs_acl)
		return ERR_PTR(-ENOMEM);

	error = xfs_attr_get(ip, ea_name, (unsigned char *)xfs_acl,
							&len, ATTR_ROOT);
	if (error) {
		 
		if (error != -ENOATTR)
			acl = ERR_PTR(error);
	} else  {
		acl = xfs_acl_from_disk(xfs_acl, len,
					XFS_ACL_MAX_ENTRIES(ip->i_mount));
	}
	kmem_free(xfs_acl);
	return acl;
}