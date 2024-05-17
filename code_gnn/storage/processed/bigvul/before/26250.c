__setscheduler(struct rq *rq, struct task_struct *p, int policy, int prio)
{
	p->policy = policy;
	p->rt_priority = prio;
	p->normal_prio = normal_prio(p);
	 
	p->prio = rt_mutex_getprio(p);
	if (rt_prio(p->prio))
		p->sched_class = &rt_sched_class;
	else
		p->sched_class = &fair_sched_class;
	set_load_weight(p);
}
