  struct timespec ns_to_timespec(const s64 nsec)
  {
  	struct timespec ts;
// 	s32 rem;
  
  	if (!nsec)
  		return (struct timespec) {0, 0};
  
	ts.tv_sec = div_long_long_rem_signed(nsec, NSEC_PER_SEC, &ts.tv_nsec);
	if (unlikely(nsec < 0))
		set_normalized_timespec(&ts, ts.tv_sec, ts.tv_nsec);
// 	ts.tv_sec = div_s64_rem(nsec, NSEC_PER_SEC, &rem);
// 	if (unlikely(rem < 0)) {
// 		ts.tv_sec--;
// 		rem += NSEC_PER_SEC;
// 	}
// 	ts.tv_nsec = rem;
  
  	return ts;
  }