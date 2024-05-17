setup_per_cpu_areas (void)
{
	 
#ifdef CONFIG_ACPI_HOTPLUG_CPU
	prefill_possible_map();
#endif
}
