static int wake_futex_pi(u32 __user *uaddr, u32 uval, struct futex_pi_state *pi_state)
{
	u32 uninitialized_var(curval), newval;
	struct task_struct *new_owner;
	bool postunlock = false;
	DEFINE_WAKE_Q(wake_q);
	int ret = 0;

	new_owner = rt_mutex_next_owner(&pi_state->pi_mutex);
	if (WARN_ON_ONCE(!new_owner)) {
		 
		ret = -EAGAIN;
		goto out_unlock;
	}

	 
	newval = FUTEX_WAITERS | task_pid_vnr(new_owner);

	if (unlikely(should_fail_futex(true)))
		ret = -EFAULT;

	if (cmpxchg_futex_value_locked(&curval, uaddr, uval, newval)) {
		ret = -EFAULT;

	} else if (curval != uval) {
		 
		if ((FUTEX_TID_MASK & curval) == uval)
			ret = -EAGAIN;
		else
			ret = -EINVAL;
	}

	if (ret)
		goto out_unlock;

	 

	raw_spin_lock(&pi_state->owner->pi_lock);
	WARN_ON(list_empty(&pi_state->list));
	list_del_init(&pi_state->list);
	raw_spin_unlock(&pi_state->owner->pi_lock);

	raw_spin_lock(&new_owner->pi_lock);
	WARN_ON(!list_empty(&pi_state->list));
	list_add(&pi_state->list, &new_owner->pi_state_list);
	pi_state->owner = new_owner;
	raw_spin_unlock(&new_owner->pi_lock);

	postunlock = __rt_mutex_futex_unlock(&pi_state->pi_mutex, &wake_q);

out_unlock:
	raw_spin_unlock_irq(&pi_state->pi_mutex.wait_lock);

	if (postunlock)
		rt_mutex_postunlock(&wake_q);

	return ret;
}
