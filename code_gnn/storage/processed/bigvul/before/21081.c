void mem_cgroup_lru_del_list(struct page *page, enum lru_list lru)
{
	struct mem_cgroup_per_zone *mz;
	struct mem_cgroup *memcg;
	struct page_cgroup *pc;

	if (mem_cgroup_disabled())
		return;

	pc = lookup_page_cgroup(page);
	memcg = pc->mem_cgroup;
	VM_BUG_ON(!memcg);
	mz = page_cgroup_zoneinfo(memcg, page);
	 
	VM_BUG_ON(MEM_CGROUP_ZSTAT(mz, lru) < (1 << compound_order(page)));
	MEM_CGROUP_ZSTAT(mz, lru) -= 1 << compound_order(page);
}