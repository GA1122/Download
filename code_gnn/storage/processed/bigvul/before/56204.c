static void armv8pmu_disable_event(struct hw_perf_event *hwc, int idx)
{
	unsigned long flags;
	struct pmu_hw_events *events = cpu_pmu->get_hw_events();

	 
	raw_spin_lock_irqsave(&events->pmu_lock, flags);

	 
	armv8pmu_disable_counter(idx);

	 
	armv8pmu_disable_intens(idx);

	raw_spin_unlock_irqrestore(&events->pmu_lock, flags);
}
