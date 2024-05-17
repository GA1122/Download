nfsd_setattr(struct svc_rqst *rqstp, struct svc_fh *fhp, struct iattr *iap,
	     int check_guard, time_t guardtime)
{
	struct dentry	*dentry;
	struct inode	*inode;
	int		accmode = NFSD_MAY_SATTR;
	umode_t		ftype = 0;
	__be32		err;
	int		host_err;
	bool		get_write_count;
	bool		size_change = (iap->ia_valid & ATTR_SIZE);

	if (iap->ia_valid & (ATTR_ATIME | ATTR_MTIME | ATTR_SIZE))
		accmode |= NFSD_MAY_WRITE|NFSD_MAY_OWNER_OVERRIDE;
	if (iap->ia_valid & ATTR_SIZE)
		ftype = S_IFREG;

	 
	get_write_count = !fhp->fh_dentry;

	 
	err = fh_verify(rqstp, fhp, ftype, accmode);
	if (err)
		return err;
	if (get_write_count) {
		host_err = fh_want_write(fhp);
		if (host_err)
			goto out;
	}

	dentry = fhp->fh_dentry;
	inode = d_inode(dentry);

	 
	if (S_ISLNK(inode->i_mode))
		iap->ia_valid &= ~ATTR_MODE;

	if (!iap->ia_valid)
		return 0;

	nfsd_sanitize_attrs(inode, iap);

	if (check_guard && guardtime != inode->i_ctime.tv_sec)
		return nfserr_notsync;

	 
	if (size_change) {
		err = nfsd_get_write_access(rqstp, fhp, iap);
		if (err)
			return err;
	}

	fh_lock(fhp);
	if (size_change) {
		 
		struct iattr size_attr = {
			.ia_valid	= ATTR_SIZE | ATTR_CTIME | ATTR_MTIME,
			.ia_size	= iap->ia_size,
		};

		host_err = notify_change(dentry, &size_attr, NULL);
		if (host_err)
			goto out_unlock;
		iap->ia_valid &= ~ATTR_SIZE;

		 
		if ((iap->ia_valid & ~ATTR_MTIME) == 0)
			goto out_unlock;
	}

	iap->ia_valid |= ATTR_CTIME;
	host_err = notify_change(dentry, iap, NULL);

out_unlock:
	fh_unlock(fhp);
	if (size_change)
		put_write_access(inode);
out:
	if (!host_err)
		host_err = commit_metadata(fhp);
	return nfserrno(host_err);
}
