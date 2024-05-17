void rpc_task_set_client(struct rpc_task *task, struct rpc_clnt *clnt)
{
	if (clnt != NULL) {
		rpc_task_release_client(task);
		task->tk_client = clnt;
		atomic_inc(&clnt->cl_count);
		if (clnt->cl_softrtry)
			task->tk_flags |= RPC_TASK_SOFT;
		 
		spin_lock(&clnt->cl_lock);
		list_add_tail(&task->tk_task, &clnt->cl_tasks);
		spin_unlock(&clnt->cl_lock);
	}
}