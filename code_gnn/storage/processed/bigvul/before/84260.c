static int futex_unlock_pi(u32 __user *uaddr, unsigned int flags)
{
	u32 uninitialized_var(curval), uval, vpid = task_pid_vnr(current);
	union futex_key key = FUTEX_KEY_INIT;
	struct futex_hash_bucket *hb;
	struct futex_q *top_waiter;
	int ret;

	if (!IS_ENABLED(CONFIG_FUTEX_PI))
		return -ENOSYS;

retry:
	if (get_user(uval, uaddr))
		return -EFAULT;
	 
	if ((uval & FUTEX_TID_MASK) != vpid)
		return -EPERM;

	ret = get_futex_key(uaddr, flags & FLAGS_SHARED, &key, VERIFY_WRITE);
	if (ret)
		return ret;

	hb = hash_futex(&key);
	spin_lock(&hb->lock);

	 
	top_waiter = futex_top_waiter(hb, &key);
	if (top_waiter) {
		struct futex_pi_state *pi_state = top_waiter->pi_state;

		ret = -EINVAL;
		if (!pi_state)
			goto out_unlock;

		 
		if (pi_state->owner != current)
			goto out_unlock;

		get_pi_state(pi_state);
		 
		raw_spin_lock_irq(&pi_state->pi_mutex.wait_lock);
		spin_unlock(&hb->lock);

		 
		ret = wake_futex_pi(uaddr, uval, pi_state);

		put_pi_state(pi_state);

		 
		if (!ret)
			goto out_putkey;
		 
		if (ret == -EFAULT)
			goto pi_faulted;
		 
		if (ret == -EAGAIN) {
			put_futex_key(&key);
			goto retry;
		}
		 
		goto out_putkey;
	}

	 
	if (cmpxchg_futex_value_locked(&curval, uaddr, uval, 0)) {
		spin_unlock(&hb->lock);
		goto pi_faulted;
	}

	 
	ret = (curval == uval) ? 0 : -EAGAIN;

out_unlock:
	spin_unlock(&hb->lock);
out_putkey:
	put_futex_key(&key);
	return ret;

pi_faulted:
	put_futex_key(&key);

	ret = fault_in_user_writeable(uaddr);
	if (!ret)
		goto retry;

	return ret;
}
