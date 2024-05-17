static void put_prev_entity(struct cfs_rq *cfs_rq, struct sched_entity *prev)
{
	 
	if (prev->on_rq)
		update_curr(cfs_rq);

	 
	check_cfs_rq_runtime(cfs_rq);

	check_spread(cfs_rq, prev);

	if (prev->on_rq) {
		update_stats_wait_start(cfs_rq, prev);
		 
		__enqueue_entity(cfs_rq, prev);
		 
		update_load_avg(cfs_rq, prev, 0);
	}
	cfs_rq->curr = NULL;
}
