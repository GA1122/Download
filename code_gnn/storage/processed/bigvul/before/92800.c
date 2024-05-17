static bool yield_to_task_fair(struct rq *rq, struct task_struct *p, bool preempt)
{
	struct sched_entity *se = &p->se;

	 
	if (!se->on_rq || throttled_hierarchy(cfs_rq_of(se)))
		return false;

	 
	set_next_buddy(se);

	yield_task_fair(rq);

	return true;
}
