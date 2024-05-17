nlmclnt_reclaim(struct nlm_host *host, struct file_lock *fl)
{
	struct nlm_rqst reqst, *req;
	int		status;

	req = &reqst;
	memset(req, 0, sizeof(*req));
	locks_init_lock(&req->a_args.lock.fl);
	locks_init_lock(&req->a_res.lock.fl);
	req->a_host  = host;
	req->a_flags = 0;

	 
	nlmclnt_setlockargs(req, fl);
	req->a_args.reclaim = 1;

	status = nlmclnt_call(nfs_file_cred(fl->fl_file), req, NLMPROC_LOCK);
	if (status >= 0 && req->a_res.status == nlm_granted)
		return 0;

	printk(KERN_WARNING "lockd: failed to reclaim lock for pid %d "
				"(errno %d, status %d)\n", fl->fl_pid,
				status, ntohl(req->a_res.status));

	 

	return -ENOLCK;
}
