static bool oom_unkillable_task(struct task_struct *p,
		const struct mem_cgroup *mem, const nodemask_t *nodemask)
{
	if (is_global_init(p))
		return true;
	if (p->flags & PF_KTHREAD)
		return true;

	 
	if (mem && !task_in_mem_cgroup(p, mem))
		return true;

	 
	if (!has_intersects_mems_allowed(p, nodemask))
		return true;

	return false;
}
