struct mem_cgroup *mem_cgroup_from_task(struct task_struct *p)
{
	 
	if (unlikely(!p))
		return NULL;

	return container_of(task_subsys_state(p, mem_cgroup_subsys_id),
				struct mem_cgroup, css);
}
