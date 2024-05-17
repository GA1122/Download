static inline u32 armv8pmu_getreset_flags(void)
{
	u32 value;

	 
	asm volatile("mrs %0, pmovsclr_el0" : "=r" (value));

	 
	value &= ARMV8_OVSR_MASK;
	asm volatile("msr pmovsclr_el0, %0" :: "r" (value));

	return value;
}
