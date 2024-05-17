static int fixup_owner(u32 __user *uaddr, struct futex_q *q, int locked)
{
	struct task_struct *owner;
	int ret = 0;

	if (locked) {
		 
		if (q->pi_state->owner != current)
			ret = fixup_pi_state_owner(uaddr, q, current);
		goto out;
	}

	 
	if (q->pi_state->owner == current) {
		 
		if (rt_mutex_trylock(&q->pi_state->pi_mutex)) {
			locked = 1;
			goto out;
		}

		 
		raw_spin_lock(&q->pi_state->pi_mutex.wait_lock);
		owner = rt_mutex_owner(&q->pi_state->pi_mutex);
		if (!owner)
			owner = rt_mutex_next_owner(&q->pi_state->pi_mutex);
		raw_spin_unlock(&q->pi_state->pi_mutex.wait_lock);
		ret = fixup_pi_state_owner(uaddr, q, owner);
		goto out;
	}

	 
	if (rt_mutex_owner(&q->pi_state->pi_mutex) == current)
		printk(KERN_ERR "fixup_owner: ret = %d pi-mutex: %p "
				"pi-state %p\n", ret,
				q->pi_state->pi_mutex.owner,
				q->pi_state->owner);

out:
	return ret ? ret : locked;
}
