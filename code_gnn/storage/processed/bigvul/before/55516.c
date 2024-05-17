static void cpuset_cpu_active(void)
{
	if (cpuhp_tasks_frozen) {
		 
		num_cpus_frozen--;
		if (likely(num_cpus_frozen)) {
			partition_sched_domains(1, NULL, NULL);
			return;
		}
		 
	}
	cpuset_update_active_cpus(true);
}
