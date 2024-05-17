static void check_enqueue_throttle(struct cfs_rq *cfs_rq)
{
	if (!cfs_bandwidth_used())
		return;

	 
	if (!cfs_rq->runtime_enabled || cfs_rq->curr)
		return;

	 
	if (cfs_rq_throttled(cfs_rq))
		return;

	 
	account_cfs_rq_runtime(cfs_rq, 0);
	if (cfs_rq->runtime_remaining <= 0)
		throttle_cfs_rq(cfs_rq);
}
