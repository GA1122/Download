static inline int armv8pmu_disable_intens(int idx)
{
	u32 counter;

	if (!armv8pmu_counter_valid(idx)) {
		pr_err("CPU%u disabling wrong PMNC counter IRQ enable %d\n",
			smp_processor_id(), idx);
		return -EINVAL;
	}

	counter = ARMV8_IDX_TO_COUNTER(idx);
	asm volatile("msr pmintenclr_el1, %0" :: "r" (BIT(counter)));
	isb();
	 
	asm volatile("msr pmovsclr_el0, %0" :: "r" (BIT(counter)));
	isb();
	return idx;
}