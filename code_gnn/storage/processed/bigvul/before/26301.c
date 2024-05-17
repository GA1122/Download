static int irqtime_account_hi_update(void)
{
	struct cpu_usage_stat *cpustat = &kstat_this_cpu.cpustat;
	unsigned long flags;
	u64 latest_ns;
	int ret = 0;

	local_irq_save(flags);
	latest_ns = this_cpu_read(cpu_hardirq_time);
	if (cputime64_gt(nsecs_to_cputime64(latest_ns), cpustat->irq))
		ret = 1;
	local_irq_restore(flags);
	return ret;
}
