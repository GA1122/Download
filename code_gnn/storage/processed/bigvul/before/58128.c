static void __setscheduler(struct rq *rq, struct task_struct *p,
			   const struct sched_attr *attr)
{
	int policy = attr->sched_policy;

	if (policy == -1)  
		policy = p->policy;

	p->policy = policy;

	if (dl_policy(policy))
		__setparam_dl(p, attr);
	else if (fair_policy(policy))
		p->static_prio = NICE_TO_PRIO(attr->sched_nice);

	 
	p->rt_priority = attr->sched_priority;

	p->normal_prio = normal_prio(p);
	p->prio = rt_mutex_getprio(p);

	if (dl_prio(p->prio))
		p->sched_class = &dl_sched_class;
	else if (rt_prio(p->prio))
		p->sched_class = &rt_sched_class;
	else
		p->sched_class = &fair_sched_class;

	set_load_weight(p);
}
