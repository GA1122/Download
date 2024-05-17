static int futex_wait_requeue_pi(u32 __user *uaddr, unsigned int flags,
				 u32 val, ktime_t *abs_time, u32 bitset,
				 u32 __user *uaddr2)
{
	struct hrtimer_sleeper timeout, *to = NULL;
	struct futex_pi_state *pi_state = NULL;
	struct rt_mutex_waiter rt_waiter;
	struct futex_hash_bucket *hb;
	union futex_key key2 = FUTEX_KEY_INIT;
	struct futex_q q = futex_q_init;
	int res, ret;

	if (!IS_ENABLED(CONFIG_FUTEX_PI))
		return -ENOSYS;

	if (uaddr == uaddr2)
		return -EINVAL;

	if (!bitset)
		return -EINVAL;

	if (abs_time) {
		to = &timeout;
		hrtimer_init_on_stack(&to->timer, (flags & FLAGS_CLOCKRT) ?
				      CLOCK_REALTIME : CLOCK_MONOTONIC,
				      HRTIMER_MODE_ABS);
		hrtimer_init_sleeper(to, current);
		hrtimer_set_expires_range_ns(&to->timer, *abs_time,
					     current->timer_slack_ns);
	}

	 
	rt_mutex_init_waiter(&rt_waiter);

	ret = get_futex_key(uaddr2, flags & FLAGS_SHARED, &key2, VERIFY_WRITE);
	if (unlikely(ret != 0))
		goto out;

	q.bitset = bitset;
	q.rt_waiter = &rt_waiter;
	q.requeue_pi_key = &key2;

	 
	ret = futex_wait_setup(uaddr, val, flags, &q, &hb);
	if (ret)
		goto out_key2;

	 
	if (match_futex(&q.key, &key2)) {
		queue_unlock(hb);
		ret = -EINVAL;
		goto out_put_keys;
	}

	 
	futex_wait_queue_me(hb, &q, to);

	spin_lock(&hb->lock);
	ret = handle_early_requeue_pi_wakeup(hb, &q, &key2, to);
	spin_unlock(&hb->lock);
	if (ret)
		goto out_put_keys;

	 

	 
	if (!q.rt_waiter) {
		 
		if (q.pi_state && (q.pi_state->owner != current)) {
			spin_lock(q.lock_ptr);
			ret = fixup_pi_state_owner(uaddr2, &q, current);
			if (ret && rt_mutex_owner(&q.pi_state->pi_mutex) == current) {
				pi_state = q.pi_state;
				get_pi_state(pi_state);
			}
			 
			put_pi_state(q.pi_state);
			spin_unlock(q.lock_ptr);
		}
	} else {
		struct rt_mutex *pi_mutex;

		 
		WARN_ON(!q.pi_state);
		pi_mutex = &q.pi_state->pi_mutex;
		ret = rt_mutex_wait_proxy_lock(pi_mutex, to, &rt_waiter);

		spin_lock(q.lock_ptr);
		if (ret && !rt_mutex_cleanup_proxy_lock(pi_mutex, &rt_waiter))
			ret = 0;

		debug_rt_mutex_free_waiter(&rt_waiter);
		 
		res = fixup_owner(uaddr2, &q, !ret);
		 
		if (res)
			ret = (res < 0) ? res : 0;

		 
		if (ret && rt_mutex_owner(&q.pi_state->pi_mutex) == current) {
			pi_state = q.pi_state;
			get_pi_state(pi_state);
		}

		 
		unqueue_me_pi(&q);
	}

	if (pi_state) {
		rt_mutex_futex_unlock(&pi_state->pi_mutex);
		put_pi_state(pi_state);
	}

	if (ret == -EINTR) {
		 
		ret = -EWOULDBLOCK;
	}

out_put_keys:
	put_futex_key(&q.key);
out_key2:
	put_futex_key(&key2);

out:
	if (to) {
		hrtimer_cancel(&to->timer);
		destroy_hrtimer_on_stack(&to->timer);
	}
	return ret;
}
