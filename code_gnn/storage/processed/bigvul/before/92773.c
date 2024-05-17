update_stats_curr_start(struct cfs_rq *cfs_rq, struct sched_entity *se)
{
	 
	se->exec_start = rq_clock_task(rq_of(cfs_rq));
}
