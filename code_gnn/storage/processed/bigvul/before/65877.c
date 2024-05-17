__be32 nfsd4_set_nfs4_label(struct svc_rqst *rqstp, struct svc_fh *fhp,
		struct xdr_netobj *label)
{
	__be32 error;
	int host_error;
	struct dentry *dentry;

	error = fh_verify(rqstp, fhp, 0  , NFSD_MAY_SATTR);
	if (error)
		return error;

	dentry = fhp->fh_dentry;

	inode_lock(d_inode(dentry));
	host_error = security_inode_setsecctx(dentry, label->data, label->len);
	inode_unlock(d_inode(dentry));
	return nfserrno(host_error);
}
