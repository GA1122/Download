static int futex_unlock_pi(u32 __user *uaddr, unsigned int flags)
{
	struct futex_hash_bucket *hb;
	struct futex_q *this, *next;
	union futex_key key = FUTEX_KEY_INIT;
	u32 uval, vpid = task_pid_vnr(current);
	int ret;

retry:
	if (get_user(uval, uaddr))
		return -EFAULT;
	 
	if ((uval & FUTEX_TID_MASK) != vpid)
		return -EPERM;

	ret = get_futex_key(uaddr, flags & FLAGS_SHARED, &key, VERIFY_WRITE);
	if (unlikely(ret != 0))
		goto out;

	hb = hash_futex(&key);
	spin_lock(&hb->lock);

	 
	if (!(uval & FUTEX_OWNER_DIED) &&
	    cmpxchg_futex_value_locked(&uval, uaddr, vpid, 0))
		goto pi_faulted;
	 
	if (unlikely(uval == vpid))
		goto out_unlock;

	 
	plist_for_each_entry_safe(this, next, &hb->chain, list) {
		if (!match_futex (&this->key, &key))
			continue;
		ret = wake_futex_pi(uaddr, uval, this);
		 
		if (ret == -EFAULT)
			goto pi_faulted;
		goto out_unlock;
	}
	 
	if (!(uval & FUTEX_OWNER_DIED)) {
		ret = unlock_futex_pi(uaddr, uval);
		if (ret == -EFAULT)
			goto pi_faulted;
	}

out_unlock:
	spin_unlock(&hb->lock);
	put_futex_key(&key);

out:
	return ret;

pi_faulted:
	spin_unlock(&hb->lock);
	put_futex_key(&key);

	ret = fault_in_user_writeable(uaddr);
	if (!ret)
		goto retry;

	return ret;
}
