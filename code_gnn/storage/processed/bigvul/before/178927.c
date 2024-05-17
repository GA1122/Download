 void jiffies_to_timeval(const unsigned long jiffies, struct timeval *value)
 {
 	 
	u64 nsec = (u64)jiffies * TICK_NSEC;
	long tv_usec;
// 	u32 rem;
  
	value->tv_sec = div_long_long_rem(nsec, NSEC_PER_SEC, &tv_usec);
	tv_usec /= NSEC_PER_USEC;
	value->tv_usec = tv_usec;
// 	value->tv_sec = div_u64_rem((u64)jiffies * TICK_NSEC,
// 				    NSEC_PER_SEC, &rem);
// 	value->tv_usec = rem / NSEC_PER_USEC;
  }