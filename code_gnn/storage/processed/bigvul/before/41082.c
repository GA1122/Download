static int futex_lock_pi(u32 __user *uaddr, unsigned int flags, int detect,
			 ktime_t *time, int trylock)
{
	struct hrtimer_sleeper timeout, *to = NULL;
	struct futex_hash_bucket *hb;
	struct futex_q q = futex_q_init;
	int res, ret;

	if (refill_pi_state_cache())
		return -ENOMEM;

	if (time) {
		to = &timeout;
		hrtimer_init_on_stack(&to->timer, CLOCK_REALTIME,
				      HRTIMER_MODE_ABS);
		hrtimer_init_sleeper(to, current);
		hrtimer_set_expires(&to->timer, *time);
	}

retry:
	ret = get_futex_key(uaddr, flags & FLAGS_SHARED, &q.key, VERIFY_WRITE);
	if (unlikely(ret != 0))
		goto out;

retry_private:
	hb = queue_lock(&q);

	ret = futex_lock_pi_atomic(uaddr, hb, &q.key, &q.pi_state, current, 0);
	if (unlikely(ret)) {
		switch (ret) {
		case 1:
			 
			ret = 0;
			goto out_unlock_put_key;
		case -EFAULT:
			goto uaddr_faulted;
		case -EAGAIN:
			 
			queue_unlock(&q, hb);
			put_futex_key(&q.key);
			cond_resched();
			goto retry;
		default:
			goto out_unlock_put_key;
		}
	}

	 
	queue_me(&q, hb);

	WARN_ON(!q.pi_state);
	 
	if (!trylock)
		ret = rt_mutex_timed_lock(&q.pi_state->pi_mutex, to, 1);
	else {
		ret = rt_mutex_trylock(&q.pi_state->pi_mutex);
		 
		ret = ret ? 0 : -EWOULDBLOCK;
	}

	spin_lock(q.lock_ptr);
	 
	res = fixup_owner(uaddr, &q, !ret);
	 
	if (res)
		ret = (res < 0) ? res : 0;

	 
	if (ret && (rt_mutex_owner(&q.pi_state->pi_mutex) == current))
		rt_mutex_unlock(&q.pi_state->pi_mutex);

	 
	unqueue_me_pi(&q);

	goto out_put_key;

out_unlock_put_key:
	queue_unlock(&q, hb);

out_put_key:
	put_futex_key(&q.key);
out:
	if (to)
		destroy_hrtimer_on_stack(&to->timer);
	return ret != -EINTR ? ret : -ERESTARTNOINTR;

uaddr_faulted:
	queue_unlock(&q, hb);

	ret = fault_in_user_writeable(uaddr);
	if (ret)
		goto out_put_key;

	if (!(flags & FLAGS_SHARED))
		goto retry_private;

	put_futex_key(&q.key);
	goto retry;
}
