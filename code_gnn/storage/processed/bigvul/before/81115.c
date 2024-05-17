static int posix_cpu_timer_del(struct k_itimer *timer)
{
	int ret = 0;
	unsigned long flags;
	struct sighand_struct *sighand;
	struct task_struct *p = timer->it.cpu.task;

	WARN_ON_ONCE(p == NULL);

	 
	sighand = lock_task_sighand(p, &flags);
	if (unlikely(sighand == NULL)) {
		 
		WARN_ON_ONCE(!list_empty(&timer->it.cpu.entry));
	} else {
		if (timer->it.cpu.firing)
			ret = TIMER_RETRY;
		else
			list_del(&timer->it.cpu.entry);

		unlock_task_sighand(p, &flags);
	}

	if (!ret)
		put_task_struct(p);

	return ret;
}
