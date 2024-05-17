  int nfsd_mountpoint(struct dentry *dentry, struct svc_export *exp)
  {
	if (d_mountpoint(dentry))
// 	if (!d_inode(dentry))
// 		return 0;
// 	if (exp->ex_flags & NFSEXP_V4ROOT)
  		return 1;
  	if (nfsd4_is_junction(dentry))
  		return 1;
	if (!(exp->ex_flags & NFSEXP_V4ROOT))
		return 0;
	return d_inode(dentry) != NULL;
// 	if (d_mountpoint(dentry))
// 		 
// 		return 2;
// 	return 0;
  }