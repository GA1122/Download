static int fixup_pi_state_owner(u32 __user *uaddr, struct futex_q *q,
				struct task_struct *argowner)
{
	struct futex_pi_state *pi_state = q->pi_state;
	u32 uval, uninitialized_var(curval), newval;
	struct task_struct *oldowner, *newowner;
	u32 newtid;
	int ret;

	lockdep_assert_held(q->lock_ptr);

	raw_spin_lock_irq(&pi_state->pi_mutex.wait_lock);

	oldowner = pi_state->owner;
	 
	if (!pi_state->owner)
		newtid |= FUTEX_OWNER_DIED;

	 
retry:
	if (!argowner) {
		if (oldowner != current) {
			 
			ret = 0;
			goto out_unlock;
		}

		if (__rt_mutex_futex_trylock(&pi_state->pi_mutex)) {
			 
			ret = 0;
			goto out_unlock;
		}

		 
		newowner = rt_mutex_owner(&pi_state->pi_mutex);
		BUG_ON(!newowner);
	} else {
		WARN_ON_ONCE(argowner != current);
		if (oldowner == current) {
			 
			ret = 0;
			goto out_unlock;
		}
		newowner = argowner;
	}

	newtid = task_pid_vnr(newowner) | FUTEX_WAITERS;

	if (get_futex_value_locked(&uval, uaddr))
		goto handle_fault;

	for (;;) {
		newval = (uval & FUTEX_OWNER_DIED) | newtid;

		if (cmpxchg_futex_value_locked(&curval, uaddr, uval, newval))
			goto handle_fault;
		if (curval == uval)
			break;
		uval = curval;
	}

	 
	if (pi_state->owner != NULL) {
		raw_spin_lock(&pi_state->owner->pi_lock);
		WARN_ON(list_empty(&pi_state->list));
		list_del_init(&pi_state->list);
		raw_spin_unlock(&pi_state->owner->pi_lock);
	}

	pi_state->owner = newowner;

	raw_spin_lock(&newowner->pi_lock);
	WARN_ON(!list_empty(&pi_state->list));
	list_add(&pi_state->list, &newowner->pi_state_list);
	raw_spin_unlock(&newowner->pi_lock);
	raw_spin_unlock_irq(&pi_state->pi_mutex.wait_lock);

	return 0;

	 
handle_fault:
	raw_spin_unlock_irq(&pi_state->pi_mutex.wait_lock);
	spin_unlock(q->lock_ptr);

	ret = fault_in_user_writeable(uaddr);

	spin_lock(q->lock_ptr);
	raw_spin_lock_irq(&pi_state->pi_mutex.wait_lock);

	 
	if (pi_state->owner != oldowner) {
		ret = 0;
		goto out_unlock;
	}

	if (ret)
		goto out_unlock;

	goto retry;

out_unlock:
	raw_spin_unlock_irq(&pi_state->pi_mutex.wait_lock);
	return ret;
}
