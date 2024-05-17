static int __init cpucache_init(void)
{
	int ret;

	 
	ret = cpuhp_setup_state(CPUHP_AP_ONLINE_DYN, "SLAB online",
				slab_online_cpu, slab_offline_cpu);
	WARN_ON(ret < 0);

	 
	slab_state = FULL;
	return 0;
}
