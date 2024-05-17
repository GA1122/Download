static inline int p4_pmu_clear_cccr_ovf(struct hw_perf_event *hwc)
{
	u64 v;

	 
	rdmsrl(hwc->config_base, v);
	if (v & P4_CCCR_OVF) {
		wrmsrl(hwc->config_base, v & ~P4_CCCR_OVF);
		return 1;
	}

	 
	rdmsrl(hwc->event_base, v);
	if (!(v & ARCH_P4_UNFLAGGED_BIT))
		return 1;

	return 0;
}
