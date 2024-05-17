static int sparc_pmu_commit_txn(struct pmu *pmu)
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	int n;

	if (!sparc_pmu)
		return -EINVAL;

	cpuc = &__get_cpu_var(cpu_hw_events);
	n = cpuc->n_events;
	if (check_excludes(cpuc->event, 0, n))
		return -EINVAL;
	if (sparc_check_constraints(cpuc->event, cpuc->events, n))
		return -EAGAIN;

	cpuc->group_flag &= ~PERF_EVENT_TXN;
	perf_pmu_enable(pmu);
	return 0;
}
