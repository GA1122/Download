static void armv8pmu_start(void)
{
	unsigned long flags;
	struct pmu_hw_events *events = cpu_pmu->get_hw_events();

	raw_spin_lock_irqsave(&events->pmu_lock, flags);
	 
	armv8pmu_pmcr_write(armv8pmu_pmcr_read() | ARMV8_PMCR_E);
	raw_spin_unlock_irqrestore(&events->pmu_lock, flags);
}