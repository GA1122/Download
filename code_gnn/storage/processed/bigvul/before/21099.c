static bool mem_cgroup_oom_lock(struct mem_cgroup *memcg)
{
	struct mem_cgroup *iter, *failed = NULL;

	for_each_mem_cgroup_tree(iter, memcg) {
		if (iter->oom_lock) {
			 
			failed = iter;
			mem_cgroup_iter_break(memcg, iter);
			break;
		} else
			iter->oom_lock = true;
	}

	if (!failed)
		return true;

	 
	for_each_mem_cgroup_tree(iter, memcg) {
		if (iter == failed) {
			mem_cgroup_iter_break(memcg, iter);
			break;
		}
		iter->oom_lock = false;
	}
	return false;
}
