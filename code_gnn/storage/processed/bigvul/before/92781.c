update_tg_cfs_util(struct cfs_rq *cfs_rq, struct sched_entity *se, struct cfs_rq *gcfs_rq)
{
	long delta = gcfs_rq->avg.util_avg - se->avg.util_avg;

	 
	if (!delta)
		return;

	 

	 
	se->avg.util_avg = gcfs_rq->avg.util_avg;
	se->avg.util_sum = se->avg.util_avg * LOAD_AVG_MAX;

	 
	add_positive(&cfs_rq->avg.util_avg, delta);
	cfs_rq->avg.util_sum = cfs_rq->avg.util_avg * LOAD_AVG_MAX;
}
