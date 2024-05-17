static int effective_prio(struct task_struct *p)
{
	p->normal_prio = normal_prio(p);
	 
	if (!rt_prio(p->prio))
		return p->normal_prio;
	return p->prio;
}
