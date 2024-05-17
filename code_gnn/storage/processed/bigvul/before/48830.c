void napi_complete_done(struct napi_struct *n, int work_done)
{
	unsigned long flags;

	 
	if (unlikely(test_bit(NAPI_STATE_NPSVC, &n->state)))
		return;

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
	if (likely(list_empty(&n->poll_list))) {
		WARN_ON_ONCE(!test_and_clear_bit(NAPI_STATE_SCHED, &n->state));
	} else {
		 
		local_irq_save(flags);
		__napi_complete(n);
		local_irq_restore(flags);
	}
}