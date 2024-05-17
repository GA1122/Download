void do_set_cpus_allowed(struct task_struct *p, const struct cpumask *new_mask)
{
	if (p->sched_class && p->sched_class->set_cpus_allowed)
		p->sched_class->set_cpus_allowed(p, new_mask);
	else {
		cpumask_copy(&p->cpus_allowed, new_mask);
		p->rt.nr_cpus_allowed = cpumask_weight(new_mask);
	}
}
