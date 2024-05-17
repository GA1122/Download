struct mem_cgroup *try_get_mem_cgroup_from_mm(struct mm_struct *mm)
{
	struct mem_cgroup *memcg = NULL;

	if (!mm)
		return NULL;
	 
	rcu_read_lock();
	do {
		memcg = mem_cgroup_from_task(rcu_dereference(mm->owner));
		if (unlikely(!memcg))
			break;
	} while (!css_tryget(&memcg->css));
	rcu_read_unlock();
	return memcg;
}
