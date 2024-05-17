bc_svc_process(struct svc_serv *serv, struct rpc_rqst *req,
	       struct svc_rqst *rqstp)
{
	struct kvec	*argv = &rqstp->rq_arg.head[0];
	struct kvec	*resv = &rqstp->rq_res.head[0];
	struct rpc_task *task;
	int proc_error;
	int error;

	dprintk("svc: %s(%p)\n", __func__, req);

	 
	rqstp->rq_xprt = serv->sv_bc_xprt;
	rqstp->rq_xid = req->rq_xid;
	rqstp->rq_prot = req->rq_xprt->prot;
	rqstp->rq_server = serv;

	rqstp->rq_addrlen = sizeof(req->rq_xprt->addr);
	memcpy(&rqstp->rq_addr, &req->rq_xprt->addr, rqstp->rq_addrlen);
	memcpy(&rqstp->rq_arg, &req->rq_rcv_buf, sizeof(rqstp->rq_arg));
	memcpy(&rqstp->rq_res, &req->rq_snd_buf, sizeof(rqstp->rq_res));

	 
	rqstp->rq_arg.len = req->rq_private_buf.len;
	if (rqstp->rq_arg.len <= rqstp->rq_arg.head[0].iov_len) {
		rqstp->rq_arg.head[0].iov_len = rqstp->rq_arg.len;
		rqstp->rq_arg.page_len = 0;
	} else if (rqstp->rq_arg.len <= rqstp->rq_arg.head[0].iov_len +
			rqstp->rq_arg.page_len)
		rqstp->rq_arg.page_len = rqstp->rq_arg.len -
			rqstp->rq_arg.head[0].iov_len;
	else
		rqstp->rq_arg.len = rqstp->rq_arg.head[0].iov_len +
			rqstp->rq_arg.page_len;

	 
	resv->iov_len = 0;

	 
	svc_getu32(argv);	 
	svc_getnl(argv);	 

	 
	proc_error = svc_process_common(rqstp, argv, resv);

	atomic_inc(&req->rq_xprt->bc_free_slots);
	if (!proc_error) {
		 
		xprt_free_bc_request(req);
		return 0;
	}

	 
	memcpy(&req->rq_snd_buf, &rqstp->rq_res, sizeof(req->rq_snd_buf));
	task = rpc_run_bc_task(req);
	if (IS_ERR(task)) {
		error = PTR_ERR(task);
		goto out;
	}

	WARN_ON_ONCE(atomic_read(&task->tk_count) != 1);
	error = task->tk_status;
	rpc_put_task(task);

out:
	dprintk("svc: %s(), error=%d\n", __func__, error);
	return error;
}