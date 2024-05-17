static int futex_requeue(u32 __user *uaddr1, unsigned int flags,
			 u32 __user *uaddr2, int nr_wake, int nr_requeue,
			 u32 *cmpval, int requeue_pi)
{
	union futex_key key1 = FUTEX_KEY_INIT, key2 = FUTEX_KEY_INIT;
	int drop_count = 0, task_count = 0, ret;
	struct futex_pi_state *pi_state = NULL;
	struct futex_hash_bucket *hb1, *hb2;
	struct plist_head *head1;
	struct futex_q *this, *next;
	u32 curval2;

	if (requeue_pi) {
		 
		if (refill_pi_state_cache())
			return -ENOMEM;
		 
		if (nr_wake != 1)
			return -EINVAL;
	}

retry:
	if (pi_state != NULL) {
		 
		free_pi_state(pi_state);
		pi_state = NULL;
	}

	ret = get_futex_key(uaddr1, flags & FLAGS_SHARED, &key1, VERIFY_READ);
	if (unlikely(ret != 0))
		goto out;
	ret = get_futex_key(uaddr2, flags & FLAGS_SHARED, &key2,
			    requeue_pi ? VERIFY_WRITE : VERIFY_READ);
	if (unlikely(ret != 0))
		goto out_put_key1;

	hb1 = hash_futex(&key1);
	hb2 = hash_futex(&key2);

retry_private:
	double_lock_hb(hb1, hb2);

	if (likely(cmpval != NULL)) {
		u32 curval;

		ret = get_futex_value_locked(&curval, uaddr1);

		if (unlikely(ret)) {
			double_unlock_hb(hb1, hb2);

			ret = get_user(curval, uaddr1);
			if (ret)
				goto out_put_keys;

			if (!(flags & FLAGS_SHARED))
				goto retry_private;

			put_futex_key(&key2);
			put_futex_key(&key1);
			goto retry;
		}
		if (curval != *cmpval) {
			ret = -EAGAIN;
			goto out_unlock;
		}
	}

	if (requeue_pi && (task_count - nr_wake < nr_requeue)) {
		 
		ret = futex_proxy_trylock_atomic(uaddr2, hb1, hb2, &key1,
						 &key2, &pi_state, nr_requeue);

		 
		if (ret == 1) {
			WARN_ON(pi_state);
			drop_count++;
			task_count++;
			ret = get_futex_value_locked(&curval2, uaddr2);
			if (!ret)
				ret = lookup_pi_state(curval2, hb2, &key2,
						      &pi_state);
		}

		switch (ret) {
		case 0:
			break;
		case -EFAULT:
			double_unlock_hb(hb1, hb2);
			put_futex_key(&key2);
			put_futex_key(&key1);
			ret = fault_in_user_writeable(uaddr2);
			if (!ret)
				goto retry;
			goto out;
		case -EAGAIN:
			 
			double_unlock_hb(hb1, hb2);
			put_futex_key(&key2);
			put_futex_key(&key1);
			cond_resched();
			goto retry;
		default:
			goto out_unlock;
		}
	}

	head1 = &hb1->chain;
	plist_for_each_entry_safe(this, next, head1, list) {
		if (task_count - nr_wake >= nr_requeue)
			break;

		if (!match_futex(&this->key, &key1))
			continue;

		 
		if ((requeue_pi && !this->rt_waiter) ||
		    (!requeue_pi && this->rt_waiter)) {
			ret = -EINVAL;
			break;
		}

		 
		if (++task_count <= nr_wake && !requeue_pi) {
			wake_futex(this);
			continue;
		}

		 
		if (requeue_pi && !match_futex(this->requeue_pi_key, &key2)) {
			ret = -EINVAL;
			break;
		}

		 
		if (requeue_pi) {
			 
			atomic_inc(&pi_state->refcount);
			this->pi_state = pi_state;
			ret = rt_mutex_start_proxy_lock(&pi_state->pi_mutex,
							this->rt_waiter,
							this->task, 1);
			if (ret == 1) {
				 
				requeue_pi_wake_futex(this, &key2, hb2);
				drop_count++;
				continue;
			} else if (ret) {
				 
				this->pi_state = NULL;
				free_pi_state(pi_state);
				goto out_unlock;
			}
		}
		requeue_futex(this, hb1, hb2, &key2);
		drop_count++;
	}

out_unlock:
	double_unlock_hb(hb1, hb2);

	 
	while (--drop_count >= 0)
		drop_futex_key_refs(&key1);

out_put_keys:
	put_futex_key(&key2);
out_put_key1:
	put_futex_key(&key1);
out:
	if (pi_state != NULL)
		free_pi_state(pi_state);
	return ret ? ret : task_count;
}