 static int semctl_setval(struct ipc_namespace *ns, int semid, int semnum,
 		unsigned long arg)
 {
 	struct sem_undo *un;
  	struct sem_array *sma;
  	struct sem* curr;
  	int err;
	int nsems;
  	struct list_head tasks;
  	int val;
  #if defined(CONFIG_64BIT) && defined(__BIG_ENDIAN)
 	 
 	val = arg >> 32;
 #else
 	 
  	val = arg;
  #endif
  
	sma = sem_lock_check(ns, semid);
	if (IS_ERR(sma))
		return PTR_ERR(sma);
// 	if (val > SEMVMX || val < 0)
// 		return -ERANGE;
  
  	INIT_LIST_HEAD(&tasks);
	nsems = sma->sem_nsems;
  
	err = -EACCES;
	if (ipcperms(ns, &sma->sem_perm, S_IWUGO))
		goto out_unlock;
// 	rcu_read_lock();
// 	sma = sem_obtain_object_check(ns, semid);
// 	if (IS_ERR(sma)) {
// 		rcu_read_unlock();
// 		return PTR_ERR(sma);
// 	}
// 
// 	if (semnum < 0 || semnum >= sma->sem_nsems) {
// 		rcu_read_unlock();
// 		return -EINVAL;
// 	}
// 
// 
// 	if (ipcperms(ns, &sma->sem_perm, S_IWUGO)) {
// 		rcu_read_unlock();
// 		return -EACCES;
// 	}
  
  	err = security_sem_semctl(sma, SETVAL);
	if (err)
		goto out_unlock;
// 	if (err) {
// 		rcu_read_unlock();
// 		return -EACCES;
// 	}
  
	err = -EINVAL;
	if(semnum < 0 || semnum >= nsems)
		goto out_unlock;
// 	sem_lock(sma, NULL, -1);
  
  	curr = &sma->sem_base[semnum];
  
	err = -ERANGE;
	if (val > SEMVMX || val < 0)
		goto out_unlock;
  	assert_spin_locked(&sma->sem_perm.lock);
  	list_for_each_entry(un, &sma->list_id, list_id)
  		un->semadj[semnum] = 0;
 
 	curr->semval = val;
 	curr->sempid = task_tgid_vnr(current);
  	sma->sem_ctime = get_seconds();
  	 
  	do_smart_update(sma, NULL, 0, 0, &tasks);
	err = 0;
out_unlock:
	sem_unlock(sma);
// 	sem_unlock(sma, -1);
  	wake_up_sem_queue_do(&tasks);
	return err;
// 	return 0;
  }