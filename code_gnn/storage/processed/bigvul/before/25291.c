xscale1pmu_write_pmnc(u32 val)
{
	 
	val &= 0xffff77f;
	asm volatile("mcr p14, 0, %0, c0, c0, 0" : : "r" (val));
}