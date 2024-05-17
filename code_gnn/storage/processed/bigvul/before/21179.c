int task_in_mem_cgroup(struct task_struct *task, const struct mem_cgroup *memcg)
{
	int ret;
	struct mem_cgroup *curr = NULL;
	struct task_struct *p;

	p = find_lock_task_mm(task);
	if (p) {
		curr = try_get_mem_cgroup_from_mm(p->mm);
		task_unlock(p);
	} else {
		 
		task_lock(task);
		curr = mem_cgroup_from_task(task);
		if (curr)
			css_get(&curr->css);
		task_unlock(task);
	}
	if (!curr)
		return 0;
	 
	ret = mem_cgroup_same_or_subtree(memcg, curr);
	css_put(&curr->css);
	return ret;
}
