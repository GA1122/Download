static inline bool skip_blocked_update(struct sched_entity *se)
{
	struct cfs_rq *gcfs_rq = group_cfs_rq(se);

	 
	if (se->avg.load_avg || se->avg.util_avg)
		return false;

	 
	if (gcfs_rq->propagate)
		return false;

	 
	return true;
}
