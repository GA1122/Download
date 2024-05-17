static enum hrtimer_restart watchdog_timer_fn(struct hrtimer *hrtimer)
{
	unsigned long touch_ts = __this_cpu_read(watchdog_touch_ts);
	struct pt_regs *regs = get_irq_regs();
	int duration;

	 
	watchdog_interrupt_count();

	 
	wake_up_process(__this_cpu_read(softlockup_watchdog));

	 
	hrtimer_forward_now(hrtimer, ns_to_ktime(get_sample_period()));

	if (touch_ts == 0) {
		if (unlikely(__this_cpu_read(softlockup_touch_sync))) {
			 
			__this_cpu_write(softlockup_touch_sync, false);
			sched_clock_tick();
		}
		__touch_watchdog();
		return HRTIMER_RESTART;
	}

	 
	duration = is_softlockup(touch_ts);
	if (unlikely(duration)) {
		 
		if (__this_cpu_read(soft_watchdog_warn) == true)
			return HRTIMER_RESTART;

		printk(KERN_ERR "BUG: soft lockup - CPU#%d stuck for %us! [%s:%d]\n",
			smp_processor_id(), duration,
			current->comm, task_pid_nr(current));
		print_modules();
		print_irqtrace_events(current);
		if (regs)
			show_regs(regs);
		else
			dump_stack();

		if (softlockup_panic)
			panic("softlockup: hung tasks");
		__this_cpu_write(soft_watchdog_warn, true);
	} else
		__this_cpu_write(soft_watchdog_warn, false);

	return HRTIMER_RESTART;
}
