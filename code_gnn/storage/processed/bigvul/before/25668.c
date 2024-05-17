static inline void sparc_pmu_enable_event(struct cpu_hw_events *cpuc, struct hw_perf_event *hwc, int idx)
{
	u64 val, mask = mask_for_index(idx);

	val = cpuc->pcr;
	val &= ~mask;
	val |= hwc->config;
	cpuc->pcr = val;

	pcr_ops->write(cpuc->pcr);
}
