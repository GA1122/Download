nlmclnt_call(struct rpc_cred *cred, struct nlm_rqst *req, u32 proc)
{
	struct nlm_host	*host = req->a_host;
	struct rpc_clnt	*clnt;
	struct nlm_args	*argp = &req->a_args;
	struct nlm_res	*resp = &req->a_res;
	struct rpc_message msg = {
		.rpc_argp	= argp,
		.rpc_resp	= resp,
		.rpc_cred	= cred,
	};
	int		status;

	dprintk("lockd: call procedure %d on %s\n",
			(int)proc, host->h_name);

	do {
		if (host->h_reclaiming && !argp->reclaim)
			goto in_grace_period;

		 
		if ((clnt = nlm_bind_host(host)) == NULL)
			return -ENOLCK;
		msg.rpc_proc = &clnt->cl_procinfo[proc];

		 
		if ((status = rpc_call_sync(clnt, &msg, 0)) < 0) {
			dprintk("lockd: rpc_call returned error %d\n", -status);
			switch (status) {
			case -EPROTONOSUPPORT:
				status = -EINVAL;
				break;
			case -ECONNREFUSED:
			case -ETIMEDOUT:
			case -ENOTCONN:
				nlm_rebind_host(host);
				status = -EAGAIN;
				break;
			case -ERESTARTSYS:
				return signalled () ? -EINTR : status;
			default:
				break;
			}
			break;
		} else
		if (resp->status == nlm_lck_denied_grace_period) {
			dprintk("lockd: server in grace period\n");
			if (argp->reclaim) {
				printk(KERN_WARNING
				     "lockd: spurious grace period reject?!\n");
				return -ENOLCK;
			}
		} else {
			if (!argp->reclaim) {
				 
				wake_up_all(&host->h_gracewait);
			}
			dprintk("lockd: server returns status %d\n", resp->status);
			return 0;	 
		}

in_grace_period:
		 
		status = nlm_wait_on_grace(&host->h_gracewait);
	} while (status == 0);

	return status;
}
