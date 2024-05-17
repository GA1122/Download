void mem_cgroup_update_page_stat(struct page *page,
				 enum mem_cgroup_page_stat_item idx, int val)
{
	struct mem_cgroup *memcg;
	struct page_cgroup *pc = lookup_page_cgroup(page);
	bool need_unlock = false;
	unsigned long uninitialized_var(flags);

	if (mem_cgroup_disabled())
		return;

	rcu_read_lock();
	memcg = pc->mem_cgroup;
	if (unlikely(!memcg || !PageCgroupUsed(pc)))
		goto out;
	 
	if (unlikely(mem_cgroup_stealed(memcg)) || PageTransHuge(page)) {
		 
		move_lock_page_cgroup(pc, &flags);
		need_unlock = true;
		memcg = pc->mem_cgroup;
		if (!memcg || !PageCgroupUsed(pc))
			goto out;
	}

	switch (idx) {
	case MEMCG_NR_FILE_MAPPED:
		if (val > 0)
			SetPageCgroupFileMapped(pc);
		else if (!page_mapped(page))
			ClearPageCgroupFileMapped(pc);
		idx = MEM_CGROUP_STAT_FILE_MAPPED;
		break;
	default:
		BUG();
	}

	this_cpu_add(memcg->stat->count[idx], val);

out:
	if (unlikely(need_unlock))
		move_unlock_page_cgroup(pc, &flags);
	rcu_read_unlock();
	return;
}