void run_posix_cpu_timers(struct task_struct *tsk)
{
	LIST_HEAD(firing);
	struct k_itimer *timer, *next;
	unsigned long flags;

	lockdep_assert_irqs_disabled();

	 
	if (!fastpath_timer_check(tsk))
		return;

	if (!lock_task_sighand(tsk, &flags))
		return;
	 
	check_thread_timers(tsk, &firing);

	check_process_timers(tsk, &firing);

	 
	unlock_task_sighand(tsk, &flags);

	 
	list_for_each_entry_safe(timer, next, &firing, it.cpu.entry) {
		int cpu_firing;

		spin_lock(&timer->it_lock);
		list_del_init(&timer->it.cpu.entry);
		cpu_firing = timer->it.cpu.firing;
		timer->it.cpu.firing = 0;
		 
		if (likely(cpu_firing >= 0))
			cpu_timer_fire(timer);
		spin_unlock(&timer->it_lock);
	}
}
