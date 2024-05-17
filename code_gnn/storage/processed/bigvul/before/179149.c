 static inline struct sem_array *sem_obtain_lock(struct ipc_namespace *ns, int id)
// static inline struct sem_array *sem_obtain_lock(struct ipc_namespace *ns,
// 			int id, struct sembuf *sops, int nsops, int *locknum)
  {
  	struct kern_ipc_perm *ipcp;
  	struct sem_array *sma;
 
 	rcu_read_lock();
 	ipcp = ipc_obtain_object(&sem_ids(ns), id);
 	if (IS_ERR(ipcp)) {
 		sma = ERR_CAST(ipcp);
  		goto err;
  	}
  
	spin_lock(&ipcp->lock);
// 	sma = container_of(ipcp, struct sem_array, sem_perm);
// 	*locknum = sem_lock(sma, sops, nsops);
  
  	 
  	if (!ipcp->deleted)
  		return container_of(ipcp, struct sem_array, sem_perm);
  
	spin_unlock(&ipcp->lock);
// 	sem_unlock(sma, *locknum);
  	sma = ERR_PTR(-EINVAL);
  err:
  	rcu_read_unlock();
 	return sma;
 }