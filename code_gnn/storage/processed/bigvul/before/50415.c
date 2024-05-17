static void __perf_cpu_hrtimer_init(struct perf_cpu_context *cpuctx, int cpu)
{
	struct hrtimer *hr = &cpuctx->hrtimer;
	struct pmu *pmu = cpuctx->ctx.pmu;
	int timer;

	 
	if (pmu->task_ctx_nr == perf_sw_context)
		return;

	 
	timer = pmu->hrtimer_interval_ms;
	if (timer < 1)
		timer = pmu->hrtimer_interval_ms = PERF_CPU_HRTIMER;

	cpuctx->hrtimer_interval = ns_to_ktime(NSEC_PER_MSEC * timer);

	hrtimer_init(hr, CLOCK_MONOTONIC, HRTIMER_MODE_REL_PINNED);
	hr->function = perf_cpu_hrtimer_handler;
}