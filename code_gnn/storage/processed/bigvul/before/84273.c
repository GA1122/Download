static void mark_wake_futex(struct wake_q_head *wake_q, struct futex_q *q)
{
	struct task_struct *p = q->task;

	if (WARN(q->pi_state || q->rt_waiter, "refusing to wake PI futex\n"))
		return;

	 
	wake_q_add(wake_q, p);
	__unqueue_futex(q);
	 
	smp_store_release(&q->lock_ptr, NULL);
}
