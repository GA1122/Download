static unsigned long get_sample_period(void)
{
	 
	return get_softlockup_thresh() * (NSEC_PER_SEC / 5);
}
