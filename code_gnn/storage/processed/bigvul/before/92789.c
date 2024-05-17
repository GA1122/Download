static inline bool vruntime_normalized(struct task_struct *p)
{
	struct sched_entity *se = &p->se;

	 
	if (p->on_rq)
		return true;

	 
	if (!se->sum_exec_runtime ||
	    (p->state == TASK_WAKING && p->sched_remote_wakeup))
		return true;

	return false;
}
