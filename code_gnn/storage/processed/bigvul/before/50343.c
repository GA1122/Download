static const char *gfs2_acl_name(int type)
{
	switch (type) {
	case ACL_TYPE_ACCESS:
		return XATTR_POSIX_ACL_ACCESS;
	case ACL_TYPE_DEFAULT:
		return XATTR_POSIX_ACL_DEFAULT;
	}
	return NULL;
}
