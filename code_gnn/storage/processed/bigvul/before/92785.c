util_est_dequeue(struct cfs_rq *cfs_rq, struct task_struct *p, bool task_sleep)
{
	long last_ewma_diff;
	struct util_est ue;

	if (!sched_feat(UTIL_EST))
		return;

	 
	ue.enqueued  = cfs_rq->avg.util_est.enqueued;
	ue.enqueued -= min_t(unsigned int, ue.enqueued, _task_util_est(p));
	WRITE_ONCE(cfs_rq->avg.util_est.enqueued, ue.enqueued);

	 
	if (!task_sleep)
		return;

	 
	ue = p->se.avg.util_est;
	if (ue.enqueued & UTIL_AVG_UNCHANGED)
		return;

	 
	ue.enqueued = (task_util(p) | UTIL_AVG_UNCHANGED);
	last_ewma_diff = ue.enqueued - ue.ewma;
	if (within_margin(last_ewma_diff, (SCHED_CAPACITY_SCALE / 100)))
		return;

	 
	ue.ewma <<= UTIL_EST_WEIGHT_SHIFT;
	ue.ewma  += last_ewma_diff;
	ue.ewma >>= UTIL_EST_WEIGHT_SHIFT;
	WRITE_ONCE(p->se.avg.util_est, ue);
}
