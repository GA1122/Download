static void watchdog_disable_all_cpus(void)
{
	int cpu;

	for_each_online_cpu(cpu)
		watchdog_disable(cpu);

	 
	watchdog_enabled = 0;
}
