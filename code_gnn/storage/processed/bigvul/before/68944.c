static int slab_offline_cpu(unsigned int cpu)
{
	 
	cancel_delayed_work_sync(&per_cpu(slab_reap_work, cpu));
	 
	per_cpu(slab_reap_work, cpu).work.func = NULL;
	return 0;
}
