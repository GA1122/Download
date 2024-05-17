SYSCALL_DEFINE3(shmget, key_t, key, size_t, size, int, shmflg)
{
	struct ipc_namespace *ns;
	struct ipc_ops shm_ops;
	struct ipc_params shm_params;

	ns = current->nsproxy->ipc_ns;

	shm_ops.getnew = newseg;
	shm_ops.associate = shm_security;
	shm_ops.more_checks = shm_more_checks;

	shm_params.key = key;
	shm_params.flg = shmflg;
	shm_params.u.size = size;

	return ipcget(ns, &shm_ids(ns), &shm_ops, &shm_params);
}
