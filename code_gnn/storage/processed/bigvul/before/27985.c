static int shm_try_destroy_current(int id, void *p, void *data)
{
	struct ipc_namespace *ns = data;
	struct kern_ipc_perm *ipcp = p;
	struct shmid_kernel *shp = container_of(ipcp, struct shmid_kernel, shm_perm);

	if (shp->shm_creator != current)
		return 0;

	 
	shp->shm_creator = NULL;

	 
	if (!ns->shm_rmid_forced)
		return 0;

	if (shm_may_destroy(ns, shp)) {
		shm_lock_by_ptr(shp);
		shm_destroy(ns, shp);
	}
	return 0;
}
