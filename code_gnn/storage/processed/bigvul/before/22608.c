static int synchronize_sched_expedited_cpu_stop(void *data)
{
	 
	smp_mb();  
	return 0;
}
