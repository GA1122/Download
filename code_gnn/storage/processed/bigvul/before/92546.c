entity_tick(struct cfs_rq *cfs_rq, struct sched_entity *curr, int queued)
{
	 
	update_curr(cfs_rq);

	 
	update_load_avg(cfs_rq, curr, UPDATE_TG);
	update_cfs_group(curr);

#ifdef CONFIG_SCHED_HRTICK
	 
	if (queued) {
		resched_curr(rq_of(cfs_rq));
		return;
	}
	 
	if (!sched_feat(DOUBLE_TICK) &&
			hrtimer_active(&rq_of(cfs_rq)->hrtick_timer))
		return;
#endif

	if (cfs_rq->nr_running > 1)
		check_preempt_tick(cfs_rq, curr);
}
