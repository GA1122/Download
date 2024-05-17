nfsd_create(struct svc_rqst *rqstp, struct svc_fh *fhp,
		char *fname, int flen, struct iattr *iap,
		int type, dev_t rdev, struct svc_fh *resfhp)
{
	struct dentry	*dentry, *dchild = NULL;
	struct inode	*dirp;
	__be32		err;
	int		host_err;

	if (isdotent(fname, flen))
		return nfserr_exist;

	err = fh_verify(rqstp, fhp, S_IFDIR, NFSD_MAY_NOP);
	if (err)
		return err;

	dentry = fhp->fh_dentry;
	dirp = d_inode(dentry);

	host_err = fh_want_write(fhp);
	if (host_err)
		return nfserrno(host_err);

	fh_lock_nested(fhp, I_MUTEX_PARENT);
	dchild = lookup_one_len(fname, dentry, flen);
	host_err = PTR_ERR(dchild);
	if (IS_ERR(dchild))
		return nfserrno(host_err);
	err = fh_compose(resfhp, fhp->fh_export, dchild, fhp);
	 
	dput(dchild);
	if (err)
		return err;
	return nfsd_create_locked(rqstp, fhp, fname, flen, iap, type,
					rdev, resfhp);
}