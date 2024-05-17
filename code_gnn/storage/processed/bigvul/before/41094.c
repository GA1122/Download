static void wake_futex(struct futex_q *q)
{
	struct task_struct *p = q->task;

	 
	get_task_struct(p);

	__unqueue_futex(q);
	 
	smp_wmb();
	q->lock_ptr = NULL;

	wake_up_state(p, TASK_NORMAL);
	put_task_struct(p);
}
