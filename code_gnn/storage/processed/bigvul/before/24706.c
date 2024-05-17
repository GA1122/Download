unsigned long clock_t_to_jiffies(unsigned long x)
{
#if (HZ % USER_HZ)==0
	if (x >= ~0UL / (HZ / USER_HZ))
		return ~0UL;
	return x * (HZ / USER_HZ);
#else
	 
	if (x >= ~0UL / HZ * USER_HZ)
		return ~0UL;

	 
	return div_u64((u64)x * HZ, USER_HZ);
#endif
}
