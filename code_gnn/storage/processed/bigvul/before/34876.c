nlmclnt_test(struct nlm_rqst *req, struct file_lock *fl)
{
	int	status;

	status = nlmclnt_call(nfs_file_cred(fl->fl_file), req, NLMPROC_TEST);
	if (status < 0)
		goto out;

	switch (req->a_res.status) {
		case nlm_granted:
			fl->fl_type = F_UNLCK;
			break;
		case nlm_lck_denied:
			 
			fl->fl_start = req->a_res.lock.fl.fl_start;
			fl->fl_end = req->a_res.lock.fl.fl_end;
			fl->fl_type = req->a_res.lock.fl.fl_type;
			fl->fl_pid = 0;
			break;
		default:
			status = nlm_stat_to_errno(req->a_res.status);
	}
out:
	nlmclnt_release_call(req);
	return status;
}