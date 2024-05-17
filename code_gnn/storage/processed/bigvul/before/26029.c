static void perf_cgroup_destroy(struct cgroup_subsys *ss,
				struct cgroup *cont)
{
	struct perf_cgroup *jc;
	jc = container_of(cgroup_subsys_state(cont, perf_subsys_id),
			  struct perf_cgroup, css);
	free_percpu(jc->info);
	kfree(jc);
}
