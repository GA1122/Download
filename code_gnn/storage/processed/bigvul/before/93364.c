static void busy_poll_stop(struct napi_struct *napi, void *have_poll_lock)
{
	int rc;

	 
	clear_bit(NAPI_STATE_MISSED, &napi->state);
	clear_bit(NAPI_STATE_IN_BUSY_POLL, &napi->state);

	local_bh_disable();

	 
	rc = napi->poll(napi, BUSY_POLL_BUDGET);
	trace_napi_poll(napi, rc, BUSY_POLL_BUDGET);
	netpoll_poll_unlock(have_poll_lock);
	if (rc == BUSY_POLL_BUDGET)
		__napi_schedule(napi);
	local_bh_enable();
}
