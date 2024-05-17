static int futex_wait_setup(u32 __user *uaddr, u32 val, int fshared,
			   struct futex_q *q, struct futex_hash_bucket **hb)
{
	u32 uval;
	int ret;

	 
retry:
	q->key = FUTEX_KEY_INIT;
	ret = get_futex_key(uaddr, fshared, &q->key);
	if (unlikely(ret != 0))
		return ret;

retry_private:
	*hb = queue_lock(q);

	ret = get_futex_value_locked(&uval, uaddr);

	if (ret) {
		queue_unlock(q, *hb);

		ret = get_user(uval, uaddr);
		if (ret)
			goto out;

		if (!fshared)
			goto retry_private;

		put_futex_key(fshared, &q->key);
		goto retry;
	}

	if (uval != val) {
		queue_unlock(q, *hb);
		ret = -EWOULDBLOCK;
	}

out:
	if (ret)
		put_futex_key(fshared, &q->key);
	return ret;
}
