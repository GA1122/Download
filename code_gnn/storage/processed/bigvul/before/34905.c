void rpc_killall_tasks(struct rpc_clnt *clnt)
{
	struct rpc_task	*rovr;


	if (list_empty(&clnt->cl_tasks))
		return;
	dprintk("RPC:       killing all tasks for client %p\n", clnt);
	 
	spin_lock(&clnt->cl_lock);
	list_for_each_entry(rovr, &clnt->cl_tasks, tk_task) {
		if (!RPC_IS_ACTIVATED(rovr))
			continue;
		if (!(rovr->tk_flags & RPC_TASK_KILLED)) {
			rovr->tk_flags |= RPC_TASK_KILLED;
			rpc_exit(rovr, -EIO);
			if (RPC_IS_QUEUED(rovr))
				rpc_wake_up_queued_task(rovr->tk_waitqueue,
							rovr);
		}
	}
	spin_unlock(&clnt->cl_lock);
}