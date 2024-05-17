static int migration_cpu_stop(void *data)
{
	struct migration_arg *arg = data;

	 
	local_irq_disable();
	__migrate_task(arg->task, raw_smp_processor_id(), arg->dest_cpu);
	local_irq_enable();
	return 0;
}
