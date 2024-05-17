static void intel_pmu_cpu_starting(int cpu)
{
	struct cpu_hw_events *cpuc = &per_cpu(cpu_hw_events, cpu);
	int core_id = topology_core_id(cpu);
	int i;

	init_debug_store_on_cpu(cpu);
	 
	intel_pmu_lbr_reset();

	if (!cpu_has_ht_siblings())
		return;

	for_each_cpu(i, topology_thread_cpumask(cpu)) {
		struct intel_percore *pc = per_cpu(cpu_hw_events, i).per_core;

		if (pc && pc->core_id == core_id) {
			kfree(cpuc->per_core);
			cpuc->per_core = pc;
			break;
		}
	}

	cpuc->per_core->core_id = core_id;
	cpuc->per_core->refcnt++;
}