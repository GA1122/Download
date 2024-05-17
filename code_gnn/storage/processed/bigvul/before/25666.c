static inline void sparc_pmu_disable_event(struct cpu_hw_events *cpuc, struct hw_perf_event *hwc, int idx)
{
	u64 mask = mask_for_index(idx);
	u64 nop = nop_for_index(idx);
	u64 val;

	val = cpuc->pcr;
	val &= ~mask;
	val |= nop;
	cpuc->pcr = val;

	pcr_ops->write(cpuc->pcr);
}
