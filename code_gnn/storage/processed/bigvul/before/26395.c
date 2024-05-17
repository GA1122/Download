static int watchdog_enable(int cpu)
{
	struct task_struct *p = per_cpu(softlockup_watchdog, cpu);
	int err = 0;

	 
	err = watchdog_nmi_enable(cpu);

	 

	 
	if (!p) {
		p = kthread_create(watchdog, (void *)(unsigned long)cpu, "watchdog/%d", cpu);
		if (IS_ERR(p)) {
			printk(KERN_ERR "softlockup watchdog for %i failed\n", cpu);
			if (!err) {
				 
				err = PTR_ERR(p);
				 
				watchdog_nmi_disable(cpu);
			}
			goto out;
		}
		kthread_bind(p, cpu);
		per_cpu(watchdog_touch_ts, cpu) = 0;
		per_cpu(softlockup_watchdog, cpu) = p;
		wake_up_process(p);
	}

out:
	return err;
}
