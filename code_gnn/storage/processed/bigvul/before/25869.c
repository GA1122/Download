static int hw_break_release_slot(int breakno)
{
	struct perf_event **pevent;
	int cpu;

	if (dbg_is_early)
		return 0;

	for_each_online_cpu(cpu) {
		pevent = per_cpu_ptr(breakinfo[breakno].pev, cpu);
		if (dbg_release_bp_slot(*pevent))
			 
			return -1;
	}
	return 0;
}
