static struct posix_acl *__gfs2_get_acl(struct inode *inode, int type)
{
	struct gfs2_inode *ip = GFS2_I(inode);
	struct posix_acl *acl;
	const char *name;
	char *data;
	int len;

	if (!ip->i_eattr)
		return NULL;

	name = gfs2_acl_name(type);
	len = gfs2_xattr_acl_get(ip, name, &data);
	if (len <= 0)
		return ERR_PTR(len);
	acl = posix_acl_from_xattr(&init_user_ns, data, len);
	kfree(data);
	return acl;
}
