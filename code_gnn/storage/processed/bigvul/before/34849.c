static struct rpc_task *__nlm_async_call(struct nlm_rqst *req, u32 proc, struct rpc_message *msg, const struct rpc_call_ops *tk_ops)
{
	struct nlm_host	*host = req->a_host;
	struct rpc_clnt	*clnt;
	struct rpc_task_setup task_setup_data = {
		.rpc_message = msg,
		.callback_ops = tk_ops,
		.callback_data = req,
		.flags = RPC_TASK_ASYNC,
	};

	dprintk("lockd: call procedure %d on %s (async)\n",
			(int)proc, host->h_name);

	 
	clnt = nlm_bind_host(host);
	if (clnt == NULL)
		goto out_err;
	msg->rpc_proc = &clnt->cl_procinfo[proc];
	task_setup_data.rpc_client = clnt;

         
	return rpc_run_task(&task_setup_data);
out_err:
	tk_ops->rpc_release(req);
	return ERR_PTR(-ENOLCK);
}