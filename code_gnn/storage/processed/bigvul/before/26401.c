static int watchdog_nmi_enable(int cpu)
{
	struct perf_event_attr *wd_attr;
	struct perf_event *event = per_cpu(watchdog_ev, cpu);

	 
	if (event && event->state > PERF_EVENT_STATE_OFF)
		goto out;

	 
	if (event != NULL)
		goto out_enable;

	wd_attr = &wd_hw_attr;
	wd_attr->sample_period = hw_nmi_get_sample_period(watchdog_thresh);
	hw_nmi_watchdog_set_attr(wd_attr);

	 
	event = perf_event_create_kernel_counter(wd_attr, cpu, NULL, watchdog_overflow_callback);
	if (!IS_ERR(event)) {
		printk(KERN_INFO "NMI watchdog enabled, takes one hw-pmu counter.\n");
		goto out_save;
	}


	 
	if (PTR_ERR(event) == -EOPNOTSUPP)
		printk(KERN_INFO "NMI watchdog disabled (cpu%i): not supported (no LAPIC?)\n", cpu);
	else if (PTR_ERR(event) == -ENOENT)
		printk(KERN_WARNING "NMI watchdog disabled (cpu%i): hardware events not enabled\n", cpu);
	else
		printk(KERN_ERR "NMI watchdog disabled (cpu%i): unable to create perf event: %ld\n", cpu, PTR_ERR(event));
	return PTR_ERR(event);

	 
out_save:
	per_cpu(watchdog_ev, cpu) = event;
out_enable:
	perf_event_enable(per_cpu(watchdog_ev, cpu));
out:
	return 0;
}
