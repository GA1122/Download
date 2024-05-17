static enum hrtimer_restart perf_cpu_hrtimer_handler(struct hrtimer *hr)
{
	struct perf_cpu_context *cpuctx;
	enum hrtimer_restart ret = HRTIMER_NORESTART;
	int rotations = 0;

	WARN_ON(!irqs_disabled());

	cpuctx = container_of(hr, struct perf_cpu_context, hrtimer);

	rotations = perf_rotate_context(cpuctx);

	 
	if (rotations) {
		hrtimer_forward_now(hr, cpuctx->hrtimer_interval);
		ret = HRTIMER_RESTART;
	}

	return ret;
}