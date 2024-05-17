svc_process(struct svc_rqst *rqstp)
{
	struct kvec		*argv = &rqstp->rq_arg.head[0];
	struct kvec		*resv = &rqstp->rq_res.head[0];
	struct svc_serv		*serv = rqstp->rq_server;
	u32			dir;

	 
	rqstp->rq_next_page = &rqstp->rq_respages[1];
	resv->iov_base = page_address(rqstp->rq_respages[0]);
	resv->iov_len = 0;
	rqstp->rq_res.pages = rqstp->rq_respages + 1;
	rqstp->rq_res.len = 0;
	rqstp->rq_res.page_base = 0;
	rqstp->rq_res.page_len = 0;
	rqstp->rq_res.buflen = PAGE_SIZE;
	rqstp->rq_res.tail[0].iov_base = NULL;
	rqstp->rq_res.tail[0].iov_len = 0;

	dir  = svc_getnl(argv);
	if (dir != 0) {
		 
		svc_printk(rqstp, "bad direction %d, dropping request\n", dir);
		serv->sv_stats->rpcbadfmt++;
		goto out_drop;
	}

	 
	if (likely(svc_process_common(rqstp, argv, resv))) {
		int ret = svc_send(rqstp);

		trace_svc_process(rqstp, ret);
		return ret;
	}
out_drop:
	trace_svc_process(rqstp, 0);
	svc_drop(rqstp);
	return 0;
}