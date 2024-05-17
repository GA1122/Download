static __be32 nfs4_get_vfs_file(struct svc_rqst *rqstp, struct nfs4_file *fp,
		struct svc_fh *cur_fh, struct nfs4_ol_stateid *stp,
		struct nfsd4_open *open)
{
	struct file *filp = NULL;
	__be32 status;
	int oflag = nfs4_access_to_omode(open->op_share_access);
	int access = nfs4_access_to_access(open->op_share_access);
	unsigned char old_access_bmap, old_deny_bmap;

	spin_lock(&fp->fi_lock);

	 
	status = nfs4_file_check_deny(fp, open->op_share_deny);
	if (status != nfs_ok) {
		spin_unlock(&fp->fi_lock);
		goto out;
	}

	 
	status = nfs4_file_get_access(fp, open->op_share_access);
	if (status != nfs_ok) {
		spin_unlock(&fp->fi_lock);
		goto out;
	}

	 
	old_access_bmap = stp->st_access_bmap;
	set_access(open->op_share_access, stp);

	 
	old_deny_bmap = stp->st_deny_bmap;
	set_deny(open->op_share_deny, stp);
	fp->fi_share_deny |= (open->op_share_deny & NFS4_SHARE_DENY_BOTH);

	if (!fp->fi_fds[oflag]) {
		spin_unlock(&fp->fi_lock);
		status = nfsd_open(rqstp, cur_fh, S_IFREG, access, &filp);
		if (status)
			goto out_put_access;
		spin_lock(&fp->fi_lock);
		if (!fp->fi_fds[oflag]) {
			fp->fi_fds[oflag] = filp;
			filp = NULL;
		}
	}
	spin_unlock(&fp->fi_lock);
	if (filp)
		fput(filp);

	status = nfsd4_truncate(rqstp, cur_fh, open);
	if (status)
		goto out_put_access;
out:
	return status;
out_put_access:
	stp->st_access_bmap = old_access_bmap;
	nfs4_file_put_access(fp, open->op_share_access);
	reset_union_bmap_deny(bmap_to_share_mode(old_deny_bmap), stp);
	goto out;
}