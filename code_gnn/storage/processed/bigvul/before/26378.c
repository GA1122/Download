static int get_softlockup_thresh(void)
{
	return watchdog_thresh * 2;
}
