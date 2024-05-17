nfsd_readlink(struct svc_rqst *rqstp, struct svc_fh *fhp, char *buf, int *lenp)
{
	mm_segment_t	oldfs;
	__be32		err;
	int		host_err;
	struct path path;

	err = fh_verify(rqstp, fhp, S_IFLNK, NFSD_MAY_NOP);
	if (err)
		goto out;

	path.mnt = fhp->fh_export->ex_path.mnt;
	path.dentry = fhp->fh_dentry;

	err = nfserr_inval;
	if (!d_is_symlink(path.dentry))
		goto out;

	touch_atime(&path);
	 

	oldfs = get_fs(); set_fs(KERNEL_DS);
	host_err = vfs_readlink(path.dentry, (char __user *)buf, *lenp);
	set_fs(oldfs);

	if (host_err < 0)
		goto out_nfserr;
	*lenp = host_err;
	err = 0;
out:
	return err;

out_nfserr:
	err = nfserrno(host_err);
	goto out;
}