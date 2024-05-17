u64 jiffies_64_to_clock_t(u64 x)
{
#if (TICK_NSEC % (NSEC_PER_SEC / USER_HZ)) == 0
# if HZ < USER_HZ
	x = div_u64(x * USER_HZ, HZ);
# elif HZ > USER_HZ
	x = div_u64(x, HZ / USER_HZ);
# else
	 
# endif
#else
	 
	x = div_u64(x * TICK_NSEC, (NSEC_PER_SEC / USER_HZ));
#endif
	return x;
}