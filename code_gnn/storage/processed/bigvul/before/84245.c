static int attach_to_pi_owner(u32 uval, union futex_key *key,
			      struct futex_pi_state **ps)
{
	pid_t pid = uval & FUTEX_TID_MASK;
	struct futex_pi_state *pi_state;
	struct task_struct *p;

	 
	if (!pid)
		return -ESRCH;
	p = futex_find_get_task(pid);
	if (!p)
		return -ESRCH;

	if (unlikely(p->flags & PF_KTHREAD)) {
		put_task_struct(p);
		return -EPERM;
	}

	 
	raw_spin_lock_irq(&p->pi_lock);
	if (unlikely(p->flags & PF_EXITING)) {
		 
		int ret = (p->flags & PF_EXITPIDONE) ? -ESRCH : -EAGAIN;

		raw_spin_unlock_irq(&p->pi_lock);
		put_task_struct(p);
		return ret;
	}

	 
	pi_state = alloc_pi_state();

	 
	rt_mutex_init_proxy_locked(&pi_state->pi_mutex, p);

	 
	pi_state->key = *key;

	WARN_ON(!list_empty(&pi_state->list));
	list_add(&pi_state->list, &p->pi_state_list);
	 
	pi_state->owner = p;
	raw_spin_unlock_irq(&p->pi_lock);

	put_task_struct(p);

	*ps = pi_state;

	return 0;
}
