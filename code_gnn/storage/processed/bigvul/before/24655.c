static int mmtimer_setup(int cpu, int comparator, unsigned long expires)
{

	switch (comparator) {
	case 0:
		mmtimer_setup_int_0(cpu, expires);
		break;
	case 1:
		mmtimer_setup_int_1(cpu, expires);
		break;
	case 2:
		mmtimer_setup_int_2(cpu, expires);
		break;
	}
	 
	if (rtc_time() <= expires)
		return 1;

	 
	return mmtimer_int_pending(comparator);
}
