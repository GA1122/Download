static void armv7pmu_stop(void)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&pmu_lock, flags);
	 
	armv7_pmnc_write(armv7_pmnc_read() & ~ARMV7_PMNC_E);
	raw_spin_unlock_irqrestore(&pmu_lock, flags);
}