xscale2pmu_read_pmnc(void)
{
	u32 val;
	asm volatile("mrc p14, 0, %0, c0, c1, 0" : "=r" (val));
	 
	return val & 0xff000009;
}
