static int __init nosoftlockup_setup(char *str)
{
	watchdog_enabled = 0;
	return 1;
}
