static void armv8pmu_enable_event(struct hw_perf_event *hwc, int idx)
{
	unsigned long flags;
	struct pmu_hw_events *events = cpu_pmu->get_hw_events();

	 
	raw_spin_lock_irqsave(&events->pmu_lock, flags);

	 
	armv8pmu_disable_counter(idx);

	 
	armv8pmu_write_evtype(idx, hwc->config_base);

	 
	armv8pmu_enable_intens(idx);

	 
	armv8pmu_enable_counter(idx);

	raw_spin_unlock_irqrestore(&events->pmu_lock, flags);
}
