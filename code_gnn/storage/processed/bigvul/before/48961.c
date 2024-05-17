__be32 inet_current_timestamp(void)
{
	u32 secs;
	u32 msecs;
	struct timespec64 ts;

	ktime_get_real_ts64(&ts);

	 
	(void)div_u64_rem(ts.tv_sec, SECONDS_PER_DAY, &secs);
	 
	msecs = secs * MSEC_PER_SEC;
	 
	msecs += (u32)ts.tv_nsec / NSEC_PER_MSEC;

	 
	return htons(msecs);
}
