static int sgi_timer_create(struct k_itimer *timer)
{
	 
	timer->it.mmtimer.clock = TIMER_OFF;
	return 0;
}
