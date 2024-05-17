xscale2pmu_write_pmnc(u32 val)
{
	 
	val &= 0xf;
	asm volatile("mcr p14, 0, %0, c0, c1, 0" : : "r" (val));
}
