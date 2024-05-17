void exit_shm(struct task_struct *task)
{
	struct ipc_namespace *ns = task->nsproxy->ipc_ns;

	if (shm_ids(ns).in_use == 0)
		return;

	 
	down_write(&shm_ids(ns).rwsem);
	if (shm_ids(ns).in_use)
		idr_for_each(&shm_ids(ns).ipcs_idr, &shm_try_destroy_current, ns);
	up_write(&shm_ids(ns).rwsem);
}