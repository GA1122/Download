static void rpc_task_set_debuginfo(struct rpc_task *task)
{
	static atomic_t rpc_pid;

	task->tk_pid = atomic_inc_return(&rpc_pid);
}
