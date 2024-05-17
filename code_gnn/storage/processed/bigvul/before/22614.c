static inline struct task_group *task_group(struct task_struct *p)
{
	struct cgroup_subsys_state *css;

	css = task_subsys_state_check(p, cpu_cgroup_subsys_id,
			lockdep_is_held(&task_rq(p)->lock));
	return container_of(css, struct task_group, css);
}
