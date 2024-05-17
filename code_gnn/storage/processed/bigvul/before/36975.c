static void __loaded_vmcs_clear(void *arg)
{
	struct loaded_vmcs *loaded_vmcs = arg;
	int cpu = raw_smp_processor_id();

	if (loaded_vmcs->cpu != cpu)
		return;  
	if (per_cpu(current_vmcs, cpu) == loaded_vmcs->vmcs)
		per_cpu(current_vmcs, cpu) = NULL;
	crash_disable_local_vmclear(cpu);
	list_del(&loaded_vmcs->loaded_vmcss_on_cpu_link);

	 
	smp_wmb();

	loaded_vmcs_init(loaded_vmcs);
	crash_enable_local_vmclear(cpu);
}
