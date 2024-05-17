nlmclnt_lock(struct nlm_rqst *req, struct file_lock *fl)
{
	struct rpc_cred *cred = nfs_file_cred(fl->fl_file);
	struct nlm_host	*host = req->a_host;
	struct nlm_res	*resp = &req->a_res;
	struct nlm_wait *block = NULL;
	unsigned char fl_flags = fl->fl_flags;
	unsigned char fl_type;
	int status = -ENOLCK;

	if (nsm_monitor(host) < 0)
		goto out;
	req->a_args.state = nsm_local_state;

	fl->fl_flags |= FL_ACCESS;
	status = do_vfs_lock(fl);
	fl->fl_flags = fl_flags;
	if (status < 0)
		goto out;

	block = nlmclnt_prepare_block(host, fl);
again:
	 
	resp->status = nlm_lck_blocked;
	for(;;) {
		 
		fl->fl_u.nfs_fl.state = host->h_state;
		status = nlmclnt_call(cred, req, NLMPROC_LOCK);
		if (status < 0)
			break;
		 
		if (resp->status ==  nlm_lck_denied_grace_period)
			continue;
		if (resp->status != nlm_lck_blocked)
			break;
		 
		status = nlmclnt_block(block, req, NLMCLNT_POLL_TIMEOUT);
		if (status < 0)
			break;
		if (resp->status != nlm_lck_blocked)
			break;
	}

	 
	if (resp->status == nlm_lck_blocked) {
		if (!req->a_args.block)
			goto out_unlock;
		if (nlmclnt_cancel(host, req->a_args.block, fl) == 0)
			goto out_unblock;
	}

	if (resp->status == nlm_granted) {
		down_read(&host->h_rwsem);
		 
		if (fl->fl_u.nfs_fl.state != host->h_state) {
			up_read(&host->h_rwsem);
			goto again;
		}
		 
		fl->fl_flags |= FL_SLEEP;
		if (do_vfs_lock(fl) < 0)
			printk(KERN_WARNING "%s: VFS is out of sync with lock manager!\n", __func__);
		up_read(&host->h_rwsem);
		fl->fl_flags = fl_flags;
		status = 0;
	}
	if (status < 0)
		goto out_unlock;
	 
	if (resp->status == nlm_lck_denied && (fl_flags & FL_SLEEP))
		status = -ENOLCK;
	else
		status = nlm_stat_to_errno(resp->status);
out_unblock:
	nlmclnt_finish_block(block);
out:
	nlmclnt_release_call(req);
	return status;
out_unlock:
	 
	dprintk("lockd: lock attempt ended in fatal error.\n"
		"       Attempting to unlock.\n");
	nlmclnt_finish_block(block);
	fl_type = fl->fl_type;
	fl->fl_type = F_UNLCK;
	down_read(&host->h_rwsem);
	do_vfs_lock(fl);
	up_read(&host->h_rwsem);
	fl->fl_type = fl_type;
	fl->fl_flags = fl_flags;
	nlmclnt_async_call(cred, req, NLMPROC_UNLOCK, &nlmclnt_unlock_ops);
	return status;
}