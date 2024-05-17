static void posix_cpu_timer_rearm(struct k_itimer *timer)
{
	struct sighand_struct *sighand;
	unsigned long flags;
	struct task_struct *p = timer->it.cpu.task;
	u64 now;

	WARN_ON_ONCE(p == NULL);

	 
	if (CPUCLOCK_PERTHREAD(timer->it_clock)) {
		cpu_clock_sample(timer->it_clock, p, &now);
		bump_cpu_timer(timer, now);
		if (unlikely(p->exit_state))
			return;

		 
		sighand = lock_task_sighand(p, &flags);
		if (!sighand)
			return;
	} else {
		 
		sighand = lock_task_sighand(p, &flags);
		if (unlikely(sighand == NULL)) {
			 
			timer->it.cpu.expires = 0;
			return;
		} else if (unlikely(p->exit_state) && thread_group_empty(p)) {
			 
			goto unlock;
		}
		cpu_timer_sample_group(timer->it_clock, p, &now);
		bump_cpu_timer(timer, now);
		 
	}

	 
	lockdep_assert_irqs_disabled();
	arm_timer(timer);
unlock:
	unlock_task_sighand(p, &flags);
}
