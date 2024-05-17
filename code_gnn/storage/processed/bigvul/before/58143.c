static void cpu_cgroup_css_offline(struct cgroup_subsys_state *css)
{
	struct task_group *tg = css_tg(css);

	sched_offline_group(tg);
}