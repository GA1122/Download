static enum hrtimer_restart napi_watchdog(struct hrtimer *timer)
{
	struct napi_struct *napi;

	napi = container_of(timer, struct napi_struct, timer);

	 
	if (napi->gro_list && !napi_disable_pending(napi) &&
	    !test_and_set_bit(NAPI_STATE_SCHED, &napi->state))
		__napi_schedule_irqoff(napi);

	return HRTIMER_NORESTART;
}