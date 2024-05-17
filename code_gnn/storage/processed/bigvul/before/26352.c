static inline struct task_group *task_group(struct task_struct *p)
{
	struct task_group *tg;
	struct cgroup_subsys_state *css;

	css = task_subsys_state_check(p, cpu_cgroup_subsys_id,
			lockdep_is_held(&p->pi_lock) ||
			lockdep_is_held(&task_rq(p)->lock));
	tg = container_of(css, struct task_group, css);

	return autogroup_task_group(p, tg);
}
