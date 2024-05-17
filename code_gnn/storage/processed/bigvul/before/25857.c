static inline void p4_pmu_disable_event(struct perf_event *event)
{
	struct hw_perf_event *hwc = &event->hw;

	 
	(void)checking_wrmsrl(hwc->config_base,
		(u64)(p4_config_unpack_cccr(hwc->config)) &
			~P4_CCCR_ENABLE & ~P4_CCCR_OVF & ~P4_CCCR_RESERVED);
}
