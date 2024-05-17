xfs_vn_ci_lookup(
	struct inode	*dir,
	struct dentry	*dentry,
	unsigned int flags)
{
	struct xfs_inode *ip;
	struct xfs_name	xname;
	struct xfs_name ci_name;
	struct qstr	dname;
	int		error;

	if (dentry->d_name.len >= MAXNAMELEN)
		return ERR_PTR(-ENAMETOOLONG);

	xfs_dentry_to_name(&xname, dentry);
	error = xfs_lookup(XFS_I(dir), &xname, &ip, &ci_name);
	if (unlikely(error)) {
		if (unlikely(error != -ENOENT))
			return ERR_PTR(error);
		 
		return NULL;
	}

	 
	if (!ci_name.name)
		return d_splice_alias(VFS_I(ip), dentry);

	 
	dname.name = ci_name.name;
	dname.len = ci_name.len;
	dentry = d_add_ci(dentry, VFS_I(ip), &dname);
	kmem_free(ci_name.name);
	return dentry;
}
