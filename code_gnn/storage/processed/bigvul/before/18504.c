void lock_memory_hotplug(void)
{
	mutex_lock(&mem_hotplug_mutex);

	 
	lock_system_sleep();
}
