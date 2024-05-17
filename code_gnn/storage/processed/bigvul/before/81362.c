static inline void trace_access_lock(int cpu)
{
	if (cpu == RING_BUFFER_ALL_CPUS) {
		 
		down_write(&all_cpu_access_lock);
	} else {
		 

		 
		down_read(&all_cpu_access_lock);

		 
		mutex_lock(&per_cpu(cpu_access_lock, cpu));
	}
}
