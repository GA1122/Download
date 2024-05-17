static void migrate_to_reboot_cpu(void)
{
  
 int cpu = 0;

	cpu_hotplug_disable();

  
 if (!cpu_online(cpu))
		cpu = cpumask_first(cpu_online_mask);

  
	current->flags |= PF_NO_SETAFFINITY;

  
	set_cpus_allowed_ptr(current, cpumask_of(cpu));
}
