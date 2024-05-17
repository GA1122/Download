nfsd4_security_inode_setsecctx(struct svc_fh *resfh, struct xdr_netobj *label, u32 *bmval)
{
	struct inode *inode = d_inode(resfh->fh_dentry);
	int status;

	inode_lock(inode);
	status = security_inode_setsecctx(resfh->fh_dentry,
		label->data, label->len);
	inode_unlock(inode);

	if (status)
		 
		bmval[2] &= ~FATTR4_WORD2_SECURITY_LABEL;

	return;
}
