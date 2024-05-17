bool sched_can_stop_tick(struct rq *rq)
{
	int fifo_nr_running;

	 
	if (rq->dl.dl_nr_running)
		return false;

	 
	if (rq->rt.rr_nr_running) {
		if (rq->rt.rr_nr_running == 1)
			return true;
		else
			return false;
	}

	 
	fifo_nr_running = rq->rt.rt_nr_running - rq->rt.rr_nr_running;
	if (fifo_nr_running)
		return true;

	 
	if (rq->nr_running > 1)
		return false;

	return true;
}
