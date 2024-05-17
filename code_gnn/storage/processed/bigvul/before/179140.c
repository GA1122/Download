 SYSCALL_DEFINE4(semtimedop, int, semid, struct sembuf __user *, tsops,
 		unsigned, nsops, const struct timespec __user *, timeout)
 {
 	int error = -EINVAL;
 	struct sem_array *sma;
  	struct sembuf fast_sops[SEMOPM_FAST];
  	struct sembuf* sops = fast_sops, *sop;
  	struct sem_undo *un;
	int undos = 0, alter = 0, max;
// 	int undos = 0, alter = 0, max, locknum;
  	struct sem_queue queue;
  	unsigned long jiffies_left = 0;
  	struct ipc_namespace *ns;
 	struct list_head tasks;
 
 	ns = current->nsproxy->ipc_ns;
 
 	if (nsops < 1 || semid < 0)
 		return -EINVAL;
 	if (nsops > ns->sc_semopm)
 		return -E2BIG;
 	if(nsops > SEMOPM_FAST) {
 		sops = kmalloc(sizeof(*sops)*nsops,GFP_KERNEL);
 		if(sops==NULL)
 			return -ENOMEM;
 	}
 	if (copy_from_user (sops, tsops, nsops * sizeof(*tsops))) {
 		error=-EFAULT;
 		goto out_free;
 	}
 	if (timeout) {
 		struct timespec _timeout;
 		if (copy_from_user(&_timeout, timeout, sizeof(*timeout))) {
 			error = -EFAULT;
 			goto out_free;
 		}
 		if (_timeout.tv_sec < 0 || _timeout.tv_nsec < 0 ||
 			_timeout.tv_nsec >= 1000000000L) {
 			error = -EINVAL;
 			goto out_free;
 		}
 		jiffies_left = timespec_to_jiffies(&_timeout);
 	}
 	max = 0;
 	for (sop = sops; sop < sops + nsops; sop++) {
 		if (sop->sem_num >= max)
 			max = sop->sem_num;
 		if (sop->sem_flg & SEM_UNDO)
 			undos = 1;
 		if (sop->sem_op != 0)
  			alter = 1;
  	}
  
// 	INIT_LIST_HEAD(&tasks);
// 
  	if (undos) {
// 		 
  		un = find_alloc_undo(ns, semid);
  		if (IS_ERR(un)) {
  			error = PTR_ERR(un);
  			goto out_free;
  		}
	} else
// 	} else {
  		un = NULL;
// 		rcu_read_lock();
// 	}
  
	INIT_LIST_HEAD(&tasks);
	rcu_read_lock();
  	sma = sem_obtain_object_check(ns, semid);
  	if (IS_ERR(sma)) {
		if (un)
			rcu_read_unlock();
// 		rcu_read_unlock();
  		error = PTR_ERR(sma);
  		goto out_free;
  	}
 
 	error = -EFBIG;
 	if (max >= sma->sem_nsems) {
 		rcu_read_unlock();
 		goto out_wakeup;
 	}
 
 	error = -EACCES;
 	if (ipcperms(ns, &sma->sem_perm, alter ? S_IWUGO : S_IRUGO)) {
 		rcu_read_unlock();
 		goto out_wakeup;
 	}
 
 	error = security_sem_semop(sma, sops, nsops, alter);
 	if (error) {
 		rcu_read_unlock();
 		goto out_wakeup;
 	}
 
 	 
  	error = -EIDRM;
	ipc_lock_object(&sma->sem_perm);
	if (un) {
		if (un->semid == -1) {
			rcu_read_unlock();
			goto out_unlock_free;
		} else {
			 
			rcu_read_unlock();
		}
	}
// 	locknum = sem_lock(sma, sops, nsops);
// 	if (un && un->semid == -1)
// 		goto out_unlock_free;
  
  	error = try_atomic_semop (sma, sops, nsops, un, task_tgid_vnr(current));
  	if (error <= 0) {
 		if (alter && error == 0)
 			do_smart_update(sma, sops, nsops, 1, &tasks);
 
 		goto out_unlock_free;
 	}
 
 	 
 		
 	queue.sops = sops;
 	queue.nsops = nsops;
 	queue.undo = un;
 	queue.pid = task_tgid_vnr(current);
 	queue.alter = alter;
 
 	if (nsops == 1) {
 		struct sem *curr;
 		curr = &sma->sem_base[sops->sem_num];
 
 		if (alter)
 			list_add_tail(&queue.list, &curr->sem_pending);
 		else
 			list_add(&queue.list, &curr->sem_pending);
 	} else {
 		if (alter)
 			list_add_tail(&queue.list, &sma->sem_pending);
 		else
 			list_add(&queue.list, &sma->sem_pending);
 		sma->complex_count++;
 	}
 
 	queue.status = -EINTR;
 	queue.sleeper = current;
  
  sleep_again:
  	current->state = TASK_INTERRUPTIBLE;
	sem_unlock(sma);
// 	sem_unlock(sma, locknum);
  
  	if (timeout)
  		jiffies_left = schedule_timeout(jiffies_left);
 	else
 		schedule();
 
 	error = get_queue_result(&queue);
 
 	if (error != -EINTR) {
 		 
 		smp_mb();
 
  		goto out_free;
  	}
  
	sma = sem_obtain_lock(ns, semid);
// 	sma = sem_obtain_lock(ns, semid, sops, nsops, &locknum);
  
  	 
 	error = get_queue_result(&queue);
 
 	 
 	if (IS_ERR(sma)) {
 		goto out_free;
 	}
 
 
 	 
 
 	if (error != -EINTR) {
 		goto out_unlock_free;
 	}
 
 	 
 	if (timeout && jiffies_left == 0)
 		error = -EAGAIN;
 
 	 
 	if (error == -EINTR && !signal_pending(current))
 		goto sleep_again;
 
  	unlink_queue(sma, &queue);
  
  out_unlock_free:
	sem_unlock(sma);
// 	sem_unlock(sma, locknum);
  out_wakeup:
  	wake_up_sem_queue_do(&tasks);
  out_free:
 	if(sops != fast_sops)
 		kfree(sops);
 	return error;
 }