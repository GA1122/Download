static void wake_futex(struct futex_q *q)
{
	struct task_struct *p = q->task;

	 
	get_task_struct(p);

	plist_del(&q->list, &q->list.plist);
	 
	smp_wmb();
	q->lock_ptr = NULL;

	wake_up_state(p, TASK_NORMAL);
	put_task_struct(p);
}
