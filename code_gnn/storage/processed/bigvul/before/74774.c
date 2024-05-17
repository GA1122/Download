mark_bsp_online (void)
{
#ifdef CONFIG_SMP
	 
	cpu_set(smp_processor_id(), cpu_online_map);
#endif
}
