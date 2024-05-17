static int fixup_owner(u32 __user *uaddr, struct futex_q *q, int locked)
{
	int ret = 0;

	if (locked) {
		 
		if (q->pi_state->owner != current)
			ret = fixup_pi_state_owner(uaddr, q, current);
		goto out;
	}

	 
	if (q->pi_state->owner == current) {
		ret = fixup_pi_state_owner(uaddr, q, NULL);
		goto out;
	}

	 
	if (rt_mutex_owner(&q->pi_state->pi_mutex) == current) {
		printk(KERN_ERR "fixup_owner: ret = %d pi-mutex: %p "
				"pi-state %p\n", ret,
				q->pi_state->pi_mutex.owner,
				q->pi_state->owner);
	}

out:
	return ret ? ret : locked;
}
