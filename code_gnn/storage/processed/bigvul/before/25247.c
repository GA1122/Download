armv6pmu_enable_event(struct hw_perf_event *hwc,
		      int idx)
{
	unsigned long val, mask, evt, flags;

	if (ARMV6_CYCLE_COUNTER == idx) {
		mask	= 0;
		evt	= ARMV6_PMCR_CCOUNT_IEN;
	} else if (ARMV6_COUNTER0 == idx) {
		mask	= ARMV6_PMCR_EVT_COUNT0_MASK;
		evt	= (hwc->config_base << ARMV6_PMCR_EVT_COUNT0_SHIFT) |
			  ARMV6_PMCR_COUNT0_IEN;
	} else if (ARMV6_COUNTER1 == idx) {
		mask	= ARMV6_PMCR_EVT_COUNT1_MASK;
		evt	= (hwc->config_base << ARMV6_PMCR_EVT_COUNT1_SHIFT) |
			  ARMV6_PMCR_COUNT1_IEN;
	} else {
		WARN_ONCE(1, "invalid counter number (%d)\n", idx);
		return;
	}

	 
	raw_spin_lock_irqsave(&pmu_lock, flags);
	val = armv6_pmcr_read();
	val &= ~mask;
	val |= evt;
	armv6_pmcr_write(val);
	raw_spin_unlock_irqrestore(&pmu_lock, flags);
}