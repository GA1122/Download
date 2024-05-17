static void arch_reinit_sched_domains(void)
{
	get_online_cpus();

	 
	partition_sched_domains(0, NULL, NULL);

	rebuild_sched_domains();
	put_online_cpus();
}
