static inline struct shmid_kernel *shm_lock(struct ipc_namespace *ns, int id)
{
	struct kern_ipc_perm *ipcp = ipc_lock(&shm_ids(ns), id);

	 
	if (IS_ERR(ipcp))
		return (void *)ipcp;
	return container_of(ipcp, struct shmid_kernel, shm_perm);
}
