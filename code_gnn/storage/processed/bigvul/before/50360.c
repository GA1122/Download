static struct posix_acl *ocfs2_get_acl_nolock(struct inode *inode,
					      int type,
					      struct buffer_head *di_bh)
{
	int name_index;
	char *value = NULL;
	struct posix_acl *acl;
	int retval;

	switch (type) {
	case ACL_TYPE_ACCESS:
		name_index = OCFS2_XATTR_INDEX_POSIX_ACL_ACCESS;
		break;
	case ACL_TYPE_DEFAULT:
		name_index = OCFS2_XATTR_INDEX_POSIX_ACL_DEFAULT;
		break;
	default:
		return ERR_PTR(-EINVAL);
	}

	retval = ocfs2_xattr_get_nolock(inode, di_bh, name_index, "", NULL, 0);
	if (retval > 0) {
		value = kmalloc(retval, GFP_NOFS);
		if (!value)
			return ERR_PTR(-ENOMEM);
		retval = ocfs2_xattr_get_nolock(inode, di_bh, name_index,
						"", value, retval);
	}

	if (retval > 0)
		acl = ocfs2_acl_from_xattr(value, retval);
	else if (retval == -ENODATA || retval == 0)
		acl = NULL;
	else
		acl = ERR_PTR(retval);

	kfree(value);

	return acl;
}
