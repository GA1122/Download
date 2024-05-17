mem_cgroup_get_reclaim_stat_from_page(struct page *page)
{
	struct page_cgroup *pc;
	struct mem_cgroup_per_zone *mz;

	if (mem_cgroup_disabled())
		return NULL;

	pc = lookup_page_cgroup(page);
	if (!PageCgroupUsed(pc))
		return NULL;
	 
	smp_rmb();
	mz = page_cgroup_zoneinfo(pc->mem_cgroup, page);
	return &mz->reclaim_stat;
}