nfs4_upgrade_open(struct svc_rqst *rqstp, struct nfs4_file *fp, struct svc_fh *cur_fh, struct nfs4_ol_stateid *stp, struct nfsd4_open *open)
{
	__be32 status;
	unsigned char old_deny_bmap = stp->st_deny_bmap;

	if (!test_access(open->op_share_access, stp))
		return nfs4_get_vfs_file(rqstp, fp, cur_fh, stp, open);

	 
	spin_lock(&fp->fi_lock);
	status = nfs4_file_check_deny(fp, open->op_share_deny);
	if (status == nfs_ok) {
		set_deny(open->op_share_deny, stp);
		fp->fi_share_deny |=
				(open->op_share_deny & NFS4_SHARE_DENY_BOTH);
	}
	spin_unlock(&fp->fi_lock);

	if (status != nfs_ok)
		return status;

	status = nfsd4_truncate(rqstp, cur_fh, open);
	if (status != nfs_ok)
		reset_union_bmap_deny(old_deny_bmap, stp);
	return status;
}