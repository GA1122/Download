bool napi_complete_done(struct napi_struct *n, int work_done)
{
	unsigned long flags, val, new;

	 
	if (unlikely(n->state & (NAPIF_STATE_NPSVC |
				 NAPIF_STATE_IN_BUSY_POLL)))
		return false;

	if (n->gro_list) {
		unsigned long timeout = 0;

		if (work_done)
			timeout = n->dev->gro_flush_timeout;

		if (timeout)
			hrtimer_start(&n->timer, ns_to_ktime(timeout),
				      HRTIMER_MODE_REL_PINNED);
		else
			napi_gro_flush(n, false);
	}
	if (unlikely(!list_empty(&n->poll_list))) {
		 
		local_irq_save(flags);
		list_del_init(&n->poll_list);
		local_irq_restore(flags);
	}

	do {
		val = READ_ONCE(n->state);

		WARN_ON_ONCE(!(val & NAPIF_STATE_SCHED));

		new = val & ~(NAPIF_STATE_MISSED | NAPIF_STATE_SCHED);

		 
		new |= (val & NAPIF_STATE_MISSED) / NAPIF_STATE_MISSED *
						    NAPIF_STATE_SCHED;
	} while (cmpxchg(&n->state, val, new) != val);

	if (unlikely(val & NAPIF_STATE_MISSED)) {
		__napi_schedule(n);
		return false;
	}

	return true;
}