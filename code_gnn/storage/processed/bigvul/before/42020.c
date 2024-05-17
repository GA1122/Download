void exit_shm(struct task_struct *task)
{
	struct ipc_namespace *ns = task->nsproxy->ipc_ns;
	struct shmid_kernel *shp, *n;

	if (list_empty(&task->sysvshm.shm_clist))
		return;

	 
	if (!ns->shm_rmid_forced) {
		down_read(&shm_ids(ns).rwsem);
		list_for_each_entry(shp, &task->sysvshm.shm_clist, shm_clist)
			shp->shm_creator = NULL;
		 
		list_del(&task->sysvshm.shm_clist);
		up_read(&shm_ids(ns).rwsem);
		return;
	}

	 
	down_write(&shm_ids(ns).rwsem);
	list_for_each_entry_safe(shp, n, &task->sysvshm.shm_clist, shm_clist) {
		shp->shm_creator = NULL;

		if (shm_may_destroy(ns, shp)) {
			shm_lock_by_ptr(shp);
			shm_destroy(ns, shp);
		}
	}

	 
	list_del(&task->sysvshm.shm_clist);
	up_write(&shm_ids(ns).rwsem);
}
