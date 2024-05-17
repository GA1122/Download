void jas_tmr_stop(jas_tmr_t *tmr)
{
	if (gettimeofday(&tmr->stop, 0)) {
		abort();
	}
}
