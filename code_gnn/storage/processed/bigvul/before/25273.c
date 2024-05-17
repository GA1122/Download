static void armv7pmu_enable_event(struct hw_perf_event *hwc, int idx)
{
	unsigned long flags;

	 
	raw_spin_lock_irqsave(&pmu_lock, flags);

	 
	armv7_pmnc_disable_counter(idx);

	 
	if (idx != ARMV7_CYCLE_COUNTER)
		armv7_pmnc_write_evtsel(idx, hwc->config_base);

	 
	armv7_pmnc_enable_intens(idx);

	 
	armv7_pmnc_enable_counter(idx);

	raw_spin_unlock_irqrestore(&pmu_lock, flags);
}
