  static inline void sem_getref(struct sem_array *sma)
  {
	spin_lock(&(sma)->sem_perm.lock);
	ipc_rcu_getref(sma);
	ipc_unlock(&(sma)->sem_perm);
// 	sem_lock(sma, NULL, -1);
// 	WARN_ON_ONCE(!ipc_rcu_getref(sma));
// 	sem_unlock(sma, -1);
  }