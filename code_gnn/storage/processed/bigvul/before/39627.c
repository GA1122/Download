static int fixup_owner(u32 __user *uaddr, int fshared, struct futex_q *q,
		       int locked)
{
	struct task_struct *owner;
	int ret = 0;

	if (locked) {
		 
		if (q->pi_state->owner != current)
			ret = fixup_pi_state_owner(uaddr, q, current, fshared);
		goto out;
	}

	 
	if (q->pi_state->owner == current) {
		 
		if (rt_mutex_trylock(&q->pi_state->pi_mutex)) {
			locked = 1;
			goto out;
		}

		 
		owner = rt_mutex_owner(&q->pi_state->pi_mutex);
		ret = fixup_pi_state_owner(uaddr, q, owner, fshared);
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
