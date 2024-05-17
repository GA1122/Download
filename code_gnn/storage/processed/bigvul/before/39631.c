static int futex_lock_pi_atomic(u32 __user *uaddr, struct futex_hash_bucket *hb,
				union futex_key *key,
				struct futex_pi_state **ps,
				struct task_struct *task, int set_waiters)
{
	int lock_taken, ret, ownerdied = 0;
	u32 uval, newval, curval;

retry:
	ret = lock_taken = 0;

	 
	newval = task_pid_vnr(task);
	if (set_waiters)
		newval |= FUTEX_WAITERS;

	curval = cmpxchg_futex_value_locked(uaddr, 0, newval);

	if (unlikely(curval == -EFAULT))
		return -EFAULT;

	 
	if ((unlikely((curval & FUTEX_TID_MASK) == task_pid_vnr(task))))
		return -EDEADLK;

	 
	if (unlikely(!curval))
		return 1;

	uval = curval;

	 
	newval = curval | FUTEX_WAITERS;

	 
	if (unlikely(ownerdied || !(curval & FUTEX_TID_MASK))) {
		 
		newval = (curval & ~FUTEX_TID_MASK) | task_pid_vnr(task);
		ownerdied = 0;
		lock_taken = 1;
	}

	curval = cmpxchg_futex_value_locked(uaddr, uval, newval);

	if (unlikely(curval == -EFAULT))
		return -EFAULT;
	if (unlikely(curval != uval))
		goto retry;

	 
	if (unlikely(lock_taken))
		return 1;

	 
	ret = lookup_pi_state(uval, hb, key, ps);

	if (unlikely(ret)) {
		switch (ret) {
		case -ESRCH:
			 
			if (get_futex_value_locked(&curval, uaddr))
				return -EFAULT;

			 
			if (curval & FUTEX_OWNER_DIED) {
				ownerdied = 1;
				goto retry;
			}
		default:
			break;
		}
	}

	return ret;
}
