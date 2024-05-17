static void attach_entity_load_avg(struct cfs_rq *cfs_rq, struct sched_entity *se, int flags)
{
	u32 divider = LOAD_AVG_MAX - 1024 + cfs_rq->avg.period_contrib;

	 
	se->avg.last_update_time = cfs_rq->avg.last_update_time;
	se->avg.period_contrib = cfs_rq->avg.period_contrib;

	 
	se->avg.util_sum = se->avg.util_avg * divider;

	se->avg.load_sum = divider;
	if (se_weight(se)) {
		se->avg.load_sum =
			div_u64(se->avg.load_avg * se->avg.load_sum, se_weight(se));
	}

	se->avg.runnable_load_sum = se->avg.load_sum;

	enqueue_load_avg(cfs_rq, se);
	cfs_rq->avg.util_avg += se->avg.util_avg;
	cfs_rq->avg.util_sum += se->avg.util_sum;

	add_tg_cfs_propagate(cfs_rq, se->avg.load_sum);

	cfs_rq_util_change(cfs_rq, flags);
}
