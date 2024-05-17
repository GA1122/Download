static void intel_pmu_disable_event(struct perf_event *event)
{
	struct hw_perf_event *hwc = &event->hw;

	if (unlikely(hwc->idx == X86_PMC_IDX_FIXED_BTS)) {
		intel_pmu_disable_bts();
		intel_pmu_drain_bts_buffer();
		return;
	}

	if (unlikely(hwc->config_base == MSR_ARCH_PERFMON_FIXED_CTR_CTRL)) {
		intel_pmu_disable_fixed(hwc);
		return;
	}

	x86_pmu_disable_event(event);

	if (unlikely(event->attr.precise_ip))
		intel_pmu_pebs_disable(event);
}