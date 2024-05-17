void run_posix_cpu_timers(struct task_struct *tsk)
{
	LIST_HEAD(firing);
	struct k_itimer *timer, *next;

	BUG_ON(!irqs_disabled());

#define UNEXPIRED(clock) \
		(cputime_eq(tsk->it_##clock##_expires, cputime_zero) || \
		 cputime_lt(clock##_ticks(tsk), tsk->it_##clock##_expires))

	if (UNEXPIRED(prof) && UNEXPIRED(virt) &&
	    (tsk->it_sched_expires == 0 ||
	     tsk->se.sum_exec_runtime < tsk->it_sched_expires))
		return;

#undef	UNEXPIRED

	 
	read_lock(&tasklist_lock);
	if (likely(tsk->signal != NULL)) {
		spin_lock(&tsk->sighand->siglock);

		 
		check_thread_timers(tsk, &firing);
		check_process_timers(tsk, &firing);

		 
		spin_unlock(&tsk->sighand->siglock);
	}
	read_unlock(&tasklist_lock);

	 
	list_for_each_entry_safe(timer, next, &firing, it.cpu.entry) {
		int firing;
		spin_lock(&timer->it_lock);
		list_del_init(&timer->it.cpu.entry);
		firing = timer->it.cpu.firing;
		timer->it.cpu.firing = 0;
		 
		if (likely(firing >= 0)) {
			cpu_timer_fire(timer);
		}
		spin_unlock(&timer->it_lock);
	}
}
