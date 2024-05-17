static int v9fs_xattr_get_acl(const struct xattr_handler *handler,
			      struct dentry *dentry, struct inode *inode,
			      const char *name, void *buffer, size_t size)
{
	struct v9fs_session_info *v9ses;
	struct posix_acl *acl;
	int error;

	v9ses = v9fs_dentry2v9ses(dentry);
	 
	if ((v9ses->flags & V9FS_ACCESS_MASK) != V9FS_ACCESS_CLIENT)
		return v9fs_xattr_get(dentry, handler->name, buffer, size);

	acl = v9fs_get_cached_acl(inode, handler->flags);
	if (IS_ERR(acl))
		return PTR_ERR(acl);
	if (acl == NULL)
		return -ENODATA;
	error = posix_acl_to_xattr(&init_user_ns, acl, buffer, size);
	posix_acl_release(acl);

	return error;
}
