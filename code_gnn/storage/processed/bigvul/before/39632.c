static int futex_proxy_trylock_atomic(u32 __user *pifutex,
				 struct futex_hash_bucket *hb1,
				 struct futex_hash_bucket *hb2,
				 union futex_key *key1, union futex_key *key2,
				 struct futex_pi_state **ps, int set_waiters)
{
	struct futex_q *top_waiter = NULL;
	u32 curval;
	int ret;

	if (get_futex_value_locked(&curval, pifutex))
		return -EFAULT;

	 
	top_waiter = futex_top_waiter(hb1, key1);

	 
	if (!top_waiter)
		return 0;

	 
	if (!match_futex(top_waiter->requeue_pi_key, key2))
		return -EINVAL;

	 
	ret = futex_lock_pi_atomic(pifutex, hb2, key2, ps, top_waiter->task,
				   set_waiters);
	if (ret == 1)
		requeue_pi_wake_futex(top_waiter, key2, hb2);

	return ret;
}
