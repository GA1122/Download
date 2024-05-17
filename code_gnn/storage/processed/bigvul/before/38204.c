static int futex_lock_pi_atomic(u32 __user *uaddr, struct futex_hash_bucket *hb,
				union futex_key *key,
				struct futex_pi_state **ps,
				struct task_struct *task, int set_waiters)
{
	int lock_taken, ret, force_take = 0;
	u32 uval, newval, curval, vpid = task_pid_vnr(task);

retry:
	ret = lock_taken = 0;

	 
	newval = vpid;
	if (set_waiters)
		newval |= FUTEX_WAITERS;

	if (unlikely(cmpxchg_futex_value_locked(&curval, uaddr, 0, newval)))
		return -EFAULT;

	 
	if ((unlikely((curval & FUTEX_TID_MASK) == vpid)))
		return -EDEADLK;

	 
	if (unlikely(!curval))
		return 1;

	uval = curval;

	 
	newval = curval | FUTEX_WAITERS;

	 
	if (unlikely(force_take)) {
		 
		newval = (curval & ~FUTEX_TID_MASK) | vpid;
		force_take = 0;
		lock_taken = 1;
	}

	if (unlikely(cmpxchg_futex_value_locked(&curval, uaddr, uval, newval)))
		return -EFAULT;
	if (unlikely(curval != uval))
		goto retry;

	 
	if (unlikely(lock_taken))
		return 1;

	 
	ret = lookup_pi_state(uval, hb, key, ps, task);

	if (unlikely(ret)) {
		switch (ret) {
		case -ESRCH:
			 
			if (get_futex_value_locked(&curval, uaddr))
				return -EFAULT;

			 
			if (!(curval & FUTEX_TID_MASK)) {
				force_take = 1;
				goto retry;
			}
		default:
			break;
		}
	}

	return ret;
}
