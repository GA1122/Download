int sched_cpu_deactivate(unsigned int cpu)
{
	int ret;

	set_cpu_active(cpu, false);
	 
	if (IS_ENABLED(CONFIG_PREEMPT))
		synchronize_rcu_mult(call_rcu, call_rcu_sched);
	else
		synchronize_rcu();

	if (!sched_smp_initialized)
		return 0;

	ret = cpuset_cpu_inactive(cpu);
	if (ret) {
		set_cpu_active(cpu, true);
		return ret;
	}
	sched_domains_numa_masks_clear(cpu);
	return 0;
}
