static void rpc_release_task(struct rpc_task *task)
{
	dprintk("RPC: %5u release task\n", task->tk_pid);

	BUG_ON (RPC_IS_QUEUED(task));

	rpc_release_resources_task(task);

	 
	if (atomic_read(&task->tk_count) != 1 + !RPC_IS_ASYNC(task)) {
		 
		if (!rpc_complete_task(task))
			return;
	} else {
		if (!atomic_dec_and_test(&task->tk_count))
			return;
	}
	rpc_final_put_task(task, task->tk_workqueue);
}
