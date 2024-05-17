void perf_sample_event_took(u64 sample_len_ns)
{
	u64 allowed_ns = ACCESS_ONCE(perf_sample_allowed_ns);
	u64 avg_local_sample_len;
	u64 local_samples_len;

	if (allowed_ns == 0)
		return;

	 
	local_samples_len = __this_cpu_read(running_sample_length);
	local_samples_len -= local_samples_len/NR_ACCUMULATED_SAMPLES;
	local_samples_len += sample_len_ns;
	__this_cpu_write(running_sample_length, local_samples_len);

	 
	avg_local_sample_len = local_samples_len/NR_ACCUMULATED_SAMPLES;

	if (avg_local_sample_len <= allowed_ns)
		return;

	if (max_samples_per_tick <= 1)
		return;

	max_samples_per_tick = DIV_ROUND_UP(max_samples_per_tick, 2);
	sysctl_perf_event_sample_rate = max_samples_per_tick * HZ;
	perf_sample_period_ns = NSEC_PER_SEC / sysctl_perf_event_sample_rate;

	update_perf_cpu_limits();

	if (!irq_work_queue(&perf_duration_work)) {
		early_printk("perf interrupt took too long (%lld > %lld), lowering "
			     "kernel.perf_event_max_sample_rate to %d\n",
			     avg_local_sample_len, allowed_ns >> 1,
			     sysctl_perf_event_sample_rate);
	}
}
