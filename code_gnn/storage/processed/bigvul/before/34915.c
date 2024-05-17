struct rpc_task *rpc_run_bc_task(struct rpc_rqst *req,
				const struct rpc_call_ops *tk_ops)
{
	struct rpc_task *task;
	struct xdr_buf *xbufp = &req->rq_snd_buf;
	struct rpc_task_setup task_setup_data = {
		.callback_ops = tk_ops,
	};

	dprintk("RPC: rpc_run_bc_task req= %p\n", req);
	 
	task = rpc_new_task(&task_setup_data);
	if (IS_ERR(task)) {
		xprt_free_bc_request(req);
		goto out;
	}
	task->tk_rqstp = req;

	 
	xbufp->len = xbufp->head[0].iov_len + xbufp->page_len +
			xbufp->tail[0].iov_len;

	task->tk_action = call_bc_transmit;
	atomic_inc(&task->tk_count);
	BUG_ON(atomic_read(&task->tk_count) != 2);
	rpc_execute(task);

out:
	dprintk("RPC: rpc_run_bc_task: task= %p\n", task);
	return task;
}