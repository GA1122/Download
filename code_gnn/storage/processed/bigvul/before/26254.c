void account_system_time(struct task_struct *p, int hardirq_offset,
			 cputime_t cputime, cputime_t cputime_scaled)
{
	struct cpu_usage_stat *cpustat = &kstat_this_cpu.cpustat;
	cputime64_t *target_cputime64;

	if ((p->flags & PF_VCPU) && (irq_count() - hardirq_offset == 0)) {
		account_guest_time(p, cputime, cputime_scaled);
		return;
	}

	if (hardirq_count() - hardirq_offset)
		target_cputime64 = &cpustat->irq;
	else if (in_serving_softirq())
		target_cputime64 = &cpustat->softirq;
	else
		target_cputime64 = &cpustat->system;

	__account_system_time(p, cputime, cputime_scaled, target_cputime64);
}
