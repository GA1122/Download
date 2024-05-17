nfs4_proc_layoutget(struct nfs4_layoutget *lgp, gfp_t gfp_flags)
{
	struct inode *inode = lgp->args.inode;
	struct nfs_server *server = NFS_SERVER(inode);
	size_t max_pages = max_response_pages(server);
	struct rpc_task *task;
	struct rpc_message msg = {
		.rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_LAYOUTGET],
		.rpc_argp = &lgp->args,
		.rpc_resp = &lgp->res,
		.rpc_cred = lgp->cred,
	};
	struct rpc_task_setup task_setup_data = {
		.rpc_client = server->client,
		.rpc_message = &msg,
		.callback_ops = &nfs4_layoutget_call_ops,
		.callback_data = lgp,
		.flags = RPC_TASK_ASYNC,
	};
	struct pnfs_layout_segment *lseg = NULL;
	int status = 0;

	dprintk("--> %s\n", __func__);

	 
	pnfs_get_layout_hdr(NFS_I(inode)->layout);

	lgp->args.layout.pages = nfs4_alloc_pages(max_pages, gfp_flags);
	if (!lgp->args.layout.pages) {
		nfs4_layoutget_release(lgp);
		return ERR_PTR(-ENOMEM);
	}
	lgp->args.layout.pglen = max_pages * PAGE_SIZE;
	lgp->args.timestamp = jiffies;

	lgp->res.layoutp = &lgp->args.layout;
	lgp->res.seq_res.sr_slot = NULL;
	nfs4_init_sequence(&lgp->args.seq_args, &lgp->res.seq_res, 0);

	task = rpc_run_task(&task_setup_data);
	if (IS_ERR(task))
		return ERR_CAST(task);
	status = nfs4_wait_for_completion_rpc_task(task);
	if (status == 0)
		status = task->tk_status;
	trace_nfs4_layoutget(lgp->args.ctx,
			&lgp->args.range,
			&lgp->res.range,
			status);
	 
	if (status == 0 && lgp->res.layoutp->len)
		lseg = pnfs_layout_process(lgp);
	rpc_put_task(task);
	dprintk("<-- %s status=%d\n", __func__, status);
	if (status)
		return ERR_PTR(status);
	return lseg;
}