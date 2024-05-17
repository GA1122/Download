static int efx_wanted_channels(void)
{
	cpumask_var_t core_mask;
	int count;
	int cpu;

	if (rss_cpus)
		return rss_cpus;

	if (unlikely(!zalloc_cpumask_var(&core_mask, GFP_KERNEL))) {
		printk(KERN_WARNING
		       "sfc: RSS disabled due to allocation failure\n");
		return 1;
	}

	count = 0;
	for_each_online_cpu(cpu) {
		if (!cpumask_test_cpu(cpu, core_mask)) {
			++count;
			cpumask_or(core_mask, core_mask,
				   topology_core_cpumask(cpu));
		}
	}

	free_cpumask_var(core_mask);
	return count;
}
