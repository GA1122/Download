nlmclnt_unlock(struct nlm_rqst *req, struct file_lock *fl)
{
	struct nlm_host	*host = req->a_host;
	struct nlm_res	*resp = &req->a_res;
	int status;
	unsigned char fl_flags = fl->fl_flags;

	 
	fl->fl_flags |= FL_EXISTS;
	down_read(&host->h_rwsem);
	status = do_vfs_lock(fl);
	up_read(&host->h_rwsem);
	fl->fl_flags = fl_flags;
	if (status == -ENOENT) {
		status = 0;
		goto out;
	}

	atomic_inc(&req->a_count);
	status = nlmclnt_async_call(nfs_file_cred(fl->fl_file), req,
			NLMPROC_UNLOCK, &nlmclnt_unlock_ops);
	if (status < 0)
		goto out;

	if (resp->status == nlm_granted)
		goto out;

	if (resp->status != nlm_lck_denied_nolocks)
		printk("lockd: unexpected unlock status: %d\n", resp->status);
	 
	status = -ENOLCK;
out:
	nlmclnt_release_call(req);
	return status;
}