static int is_softlockup(unsigned long touch_ts)
{
	unsigned long now = get_timestamp(smp_processor_id());

	 
	if (time_after(now, touch_ts + get_softlockup_thresh()))
		return now - touch_ts;

	return 0;
}
