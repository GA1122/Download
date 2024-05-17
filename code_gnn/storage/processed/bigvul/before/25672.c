static void sparc_pmu_start_txn(struct pmu *pmu)
{
	struct cpu_hw_events *cpuhw = &__get_cpu_var(cpu_hw_events);

	perf_pmu_disable(pmu);
	cpuhw->group_flag |= PERF_EVENT_TXN;
}
