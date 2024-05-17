compute_energy(struct task_struct *p, int dst_cpu, struct perf_domain *pd)
{
	long util, max_util, sum_util, energy = 0;
	int cpu;

	for (; pd; pd = pd->next) {
		max_util = sum_util = 0;
		 
		for_each_cpu_and(cpu, perf_domain_span(pd), cpu_online_mask) {
			util = cpu_util_next(cpu, p, dst_cpu);
			util = schedutil_energy_util(cpu, util);
			max_util = max(util, max_util);
			sum_util += util;
		}

		energy += em_pd_energy(pd->em_pd, max_util, sum_util);
	}

	return energy;
}
