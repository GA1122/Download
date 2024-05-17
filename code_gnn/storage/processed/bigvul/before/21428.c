static void __mem_cgroup_commit_charge(struct mem_cgroup *memcg,
				       struct page *page,
				       unsigned int nr_pages,
				       struct page_cgroup *pc,
				       enum charge_type ctype)
{
	lock_page_cgroup(pc);
	if (unlikely(PageCgroupUsed(pc))) {
		unlock_page_cgroup(pc);
		__mem_cgroup_cancel_charge(memcg, nr_pages);
		return;
	}
	 
	pc->mem_cgroup = memcg;
	 
	smp_wmb();
	switch (ctype) {
	case MEM_CGROUP_CHARGE_TYPE_CACHE:
	case MEM_CGROUP_CHARGE_TYPE_SHMEM:
		SetPageCgroupCache(pc);
		SetPageCgroupUsed(pc);
		break;
	case MEM_CGROUP_CHARGE_TYPE_MAPPED:
		ClearPageCgroupCache(pc);
		SetPageCgroupUsed(pc);
		break;
	default:
		break;
	}

	mem_cgroup_charge_statistics(memcg, PageCgroupCache(pc), nr_pages);
	unlock_page_cgroup(pc);
	WARN_ON_ONCE(PageLRU(page));
	 
	memcg_check_events(memcg, page);
}
