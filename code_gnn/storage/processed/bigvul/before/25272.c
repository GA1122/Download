static void armv7pmu_disable_event(struct hw_perf_event *hwc, int idx)
{
	unsigned long flags;

	 
	raw_spin_lock_irqsave(&pmu_lock, flags);

	 
	armv7_pmnc_disable_counter(idx);

	 
	armv7_pmnc_disable_intens(idx);

	raw_spin_unlock_irqrestore(&pmu_lock, flags);
}
