static void fsl_emb_pmu_del(struct perf_event *event, int flags)
{
	struct cpu_hw_events *cpuhw;
	int i = event->hw.idx;

	perf_pmu_disable(event->pmu);
	if (i < 0)
		goto out;

	fsl_emb_pmu_read(event);

	cpuhw = &get_cpu_var(cpu_hw_events);

	WARN_ON(event != cpuhw->event[event->hw.idx]);

	write_pmlca(i, 0);
	write_pmlcb(i, 0);
	write_pmc(i, 0);

	cpuhw->event[i] = NULL;
	event->hw.idx = -1;

	 

	cpuhw->n_events--;

 out:
	perf_pmu_enable(event->pmu);
	put_cpu_var(cpu_hw_events);
}