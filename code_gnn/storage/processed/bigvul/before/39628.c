static int fixup_pi_state_owner(u32 __user *uaddr, struct futex_q *q,
				struct task_struct *newowner, int fshared)
{
	u32 newtid = task_pid_vnr(newowner) | FUTEX_WAITERS;
	struct futex_pi_state *pi_state = q->pi_state;
	struct task_struct *oldowner = pi_state->owner;
	u32 uval, curval, newval;
	int ret;

	 
	if (!pi_state->owner)
		newtid |= FUTEX_OWNER_DIED;

	 
retry:
	if (get_futex_value_locked(&uval, uaddr))
		goto handle_fault;

	while (1) {
		newval = (uval & FUTEX_OWNER_DIED) | newtid;

		curval = cmpxchg_futex_value_locked(uaddr, uval, newval);

		if (curval == -EFAULT)
			goto handle_fault;
		if (curval == uval)
			break;
		uval = curval;
	}

	 
	if (pi_state->owner != NULL) {
		raw_spin_lock_irq(&pi_state->owner->pi_lock);
		WARN_ON(list_empty(&pi_state->list));
		list_del_init(&pi_state->list);
		raw_spin_unlock_irq(&pi_state->owner->pi_lock);
	}

	pi_state->owner = newowner;

	raw_spin_lock_irq(&newowner->pi_lock);
	WARN_ON(!list_empty(&pi_state->list));
	list_add(&pi_state->list, &newowner->pi_state_list);
	raw_spin_unlock_irq(&newowner->pi_lock);
	return 0;

	 
handle_fault:
	spin_unlock(q->lock_ptr);

	ret = fault_in_user_writeable(uaddr);

	spin_lock(q->lock_ptr);

	 
	if (pi_state->owner != oldowner)
		return 0;

	if (ret)
		return ret;

	goto retry;
}
