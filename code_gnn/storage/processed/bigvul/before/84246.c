static int attach_to_pi_state(u32 __user *uaddr, u32 uval,
			      struct futex_pi_state *pi_state,
			      struct futex_pi_state **ps)
{
	pid_t pid = uval & FUTEX_TID_MASK;
	u32 uval2;
	int ret;

	 
	if (unlikely(!pi_state))
		return -EINVAL;

	 
	WARN_ON(!atomic_read(&pi_state->refcount));

	 
	raw_spin_lock_irq(&pi_state->pi_mutex.wait_lock);

	 
	if (get_futex_value_locked(&uval2, uaddr))
		goto out_efault;

	if (uval != uval2)
		goto out_eagain;

	 
	if (uval & FUTEX_OWNER_DIED) {
		 
		if (!pi_state->owner) {
			 
			if (pid)
				goto out_einval;
			 
			goto out_attach;
		}

		 
		if (!pid)
			goto out_attach;
	} else {
		 
		if (!pi_state->owner)
			goto out_einval;
	}

	 
	if (pid != task_pid_vnr(pi_state->owner))
		goto out_einval;

out_attach:
	get_pi_state(pi_state);
	raw_spin_unlock_irq(&pi_state->pi_mutex.wait_lock);
	*ps = pi_state;
	return 0;

out_einval:
	ret = -EINVAL;
	goto out_error;

out_eagain:
	ret = -EAGAIN;
	goto out_error;

out_efault:
	ret = -EFAULT;
	goto out_error;

out_error:
	raw_spin_unlock_irq(&pi_state->pi_mutex.wait_lock);
	return ret;
}
