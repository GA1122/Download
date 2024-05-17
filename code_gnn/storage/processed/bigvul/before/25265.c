static inline u32 armv7_pmnc_getreset_flags(void)
{
	u32 val;

	 
	asm volatile("mrc p15, 0, %0, c9, c12, 3" : "=r" (val));

	 
	val &= ARMV7_FLAG_MASK;
	asm volatile("mcr p15, 0, %0, c9, c12, 3" : : "r" (val));

	return val;
}
