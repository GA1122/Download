static __be32 nfsd_check_obj_isreg(struct svc_fh *fh)
{
	umode_t mode = d_inode(fh->fh_dentry)->i_mode;

	if (S_ISREG(mode))
		return nfs_ok;
	if (S_ISDIR(mode))
		return nfserr_isdir;
	 
	return nfserr_symlink;
}
