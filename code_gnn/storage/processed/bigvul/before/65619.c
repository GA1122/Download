nfsd4_process_open2(struct svc_rqst *rqstp, struct svc_fh *current_fh, struct nfsd4_open *open)
{
	struct nfsd4_compoundres *resp = rqstp->rq_resp;
	struct nfs4_client *cl = open->op_openowner->oo_owner.so_client;
	struct nfs4_file *fp = NULL;
	struct nfs4_ol_stateid *stp = NULL;
	struct nfs4_delegation *dp = NULL;
	__be32 status;

	 
	fp = find_or_add_file(open->op_file, &current_fh->fh_handle);
	if (fp != open->op_file) {
		status = nfs4_check_deleg(cl, open, &dp);
		if (status)
			goto out;
		spin_lock(&fp->fi_lock);
		stp = nfsd4_find_existing_open(fp, open);
		spin_unlock(&fp->fi_lock);
	} else {
		open->op_file = NULL;
		status = nfserr_bad_stateid;
		if (nfsd4_is_deleg_cur(open))
			goto out;
	}

	 
	if (stp) {
		 
		mutex_lock(&stp->st_mutex);
		status = nfs4_upgrade_open(rqstp, fp, current_fh, stp, open);
		if (status) {
			mutex_unlock(&stp->st_mutex);
			goto out;
		}
	} else {
		 
		stp = init_open_stateid(fp, open);
		 
		if (stp->st_access_bmap != 0) {
			status = nfs4_upgrade_open(rqstp, fp, current_fh,
						stp, open);
			if (status) {
				mutex_unlock(&stp->st_mutex);
				goto out;
			}
			goto upgrade_out;
		}
		status = nfs4_get_vfs_file(rqstp, fp, current_fh, stp, open);
		if (status) {
			mutex_unlock(&stp->st_mutex);
			release_open_stateid(stp);
			goto out;
		}

		stp->st_clnt_odstate = find_or_hash_clnt_odstate(fp,
							open->op_odstate);
		if (stp->st_clnt_odstate == open->op_odstate)
			open->op_odstate = NULL;
	}
upgrade_out:
	nfs4_inc_and_copy_stateid(&open->op_stateid, &stp->st_stid);
	mutex_unlock(&stp->st_mutex);

	if (nfsd4_has_session(&resp->cstate)) {
		if (open->op_deleg_want & NFS4_SHARE_WANT_NO_DELEG) {
			open->op_delegate_type = NFS4_OPEN_DELEGATE_NONE_EXT;
			open->op_why_no_deleg = WND4_NOT_WANTED;
			goto nodeleg;
		}
	}

	 
	nfs4_open_delegation(current_fh, open, stp);
nodeleg:
	status = nfs_ok;

	dprintk("%s: stateid=" STATEID_FMT "\n", __func__,
		STATEID_VAL(&stp->st_stid.sc_stateid));
out:
	 
	if (open->op_delegate_type == NFS4_OPEN_DELEGATE_NONE && dp &&
	    open->op_deleg_want)
		nfsd4_deleg_xgrade_none_ext(open, dp);

	if (fp)
		put_nfs4_file(fp);
	if (status == 0 && open->op_claim_type == NFS4_OPEN_CLAIM_PREVIOUS)
		open->op_openowner->oo_flags |= NFS4_OO_CONFIRMED;
	 
	open->op_rflags = NFS4_OPEN_RESULT_LOCKTYPE_POSIX;
	if (nfsd4_has_session(&resp->cstate))
		open->op_rflags |= NFS4_OPEN_RESULT_MAY_NOTIFY_LOCK;
	else if (!(open->op_openowner->oo_flags & NFS4_OO_CONFIRMED))
		open->op_rflags |= NFS4_OPEN_RESULT_CONFIRM;

	if (dp)
		nfs4_put_stid(&dp->dl_stid);
	if (stp)
		nfs4_put_stid(&stp->st_stid);

	return status;
}
