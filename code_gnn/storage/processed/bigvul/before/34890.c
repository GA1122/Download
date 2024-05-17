call_timeout(struct rpc_task *task)
{
	struct rpc_clnt	*clnt = task->tk_client;

	if (xprt_adjust_timeout(task->tk_rqstp) == 0) {
		dprintk("RPC: %5u call_timeout (minor)\n", task->tk_pid);
		goto retry;
	}

	dprintk("RPC: %5u call_timeout (major)\n", task->tk_pid);
	task->tk_timeouts++;

	if (RPC_IS_SOFTCONN(task)) {
		rpc_exit(task, -ETIMEDOUT);
		return;
	}
	if (RPC_IS_SOFT(task)) {
		if (clnt->cl_chatty)
			printk(KERN_NOTICE "%s: server %s not responding, timed out\n",
				clnt->cl_protname, clnt->cl_server);
		if (task->tk_flags & RPC_TASK_TIMEOUT)
			rpc_exit(task, -ETIMEDOUT);
		else
			rpc_exit(task, -EIO);
		return;
	}

	if (!(task->tk_flags & RPC_CALL_MAJORSEEN)) {
		task->tk_flags |= RPC_CALL_MAJORSEEN;
		if (clnt->cl_chatty)
			printk(KERN_NOTICE "%s: server %s not responding, still trying\n",
			clnt->cl_protname, clnt->cl_server);
	}
	rpc_force_rebind(clnt);
	 
	rpcauth_invalcred(task);

retry:
	clnt->cl_stats->rpcretrans++;
	task->tk_action = call_bind;
	task->tk_status = 0;
}