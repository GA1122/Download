lookup_pi_state(u32 uval, struct futex_hash_bucket *hb,
		union futex_key *key, struct futex_pi_state **ps)
{
	struct futex_pi_state *pi_state = NULL;
	struct futex_q *this, *next;
	struct plist_head *head;
	struct task_struct *p;
	pid_t pid = uval & FUTEX_TID_MASK;

	head = &hb->chain;

	plist_for_each_entry_safe(this, next, head, list) {
		if (match_futex(&this->key, key)) {
			 
			pi_state = this->pi_state;
			 
			if (unlikely(!pi_state))
				return -EINVAL;

			WARN_ON(!atomic_read(&pi_state->refcount));

			 
			if (pid && pi_state->owner) {
				 
				if (pid != task_pid_vnr(pi_state->owner))
					return -EINVAL;
			}

			atomic_inc(&pi_state->refcount);
			*ps = pi_state;

			return 0;
		}
	}

	 
	if (!pid)
		return -ESRCH;
	p = futex_find_get_task(pid);
	if (!p)
		return -ESRCH;

	 
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
