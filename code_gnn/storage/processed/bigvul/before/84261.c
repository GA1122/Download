static void futex_wait_queue_me(struct futex_hash_bucket *hb, struct futex_q *q,
				struct hrtimer_sleeper *timeout)
{
	 
	set_current_state(TASK_INTERRUPTIBLE);
	queue_me(q, hb);

	 
	if (timeout)
		hrtimer_start_expires(&timeout->timer, HRTIMER_MODE_ABS);

	 
	if (likely(!plist_node_empty(&q->list))) {
		 
		if (!timeout || timeout->task)
			freezable_schedule();
	}
	__set_current_state(TASK_RUNNING);
}
