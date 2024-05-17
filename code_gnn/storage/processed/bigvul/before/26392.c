static int watchdog(void *unused)
{
	static struct sched_param param = { .sched_priority = MAX_RT_PRIO-1 };
	struct hrtimer *hrtimer = &__raw_get_cpu_var(watchdog_hrtimer);

	sched_setscheduler(current, SCHED_FIFO, &param);

	 
	__touch_watchdog();

	 
	 
	hrtimer_start(hrtimer, ns_to_ktime(get_sample_period()),
		      HRTIMER_MODE_REL_PINNED);

	set_current_state(TASK_INTERRUPTIBLE);
	 
	while (!kthread_should_stop()) {
		__touch_watchdog();
		schedule();

		if (kthread_should_stop())
			break;

		set_current_state(TASK_INTERRUPTIBLE);
	}
	__set_current_state(TASK_RUNNING);

	return 0;
}
