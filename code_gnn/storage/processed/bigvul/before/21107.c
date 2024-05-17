int mem_cgroup_prepare_migration(struct page *page,
	struct page *newpage, struct mem_cgroup **memcgp, gfp_t gfp_mask)
{
	struct mem_cgroup *memcg = NULL;
	struct page_cgroup *pc;
	enum charge_type ctype;
	int ret = 0;

	*memcgp = NULL;

	VM_BUG_ON(PageTransHuge(page));
	if (mem_cgroup_disabled())
		return 0;

	pc = lookup_page_cgroup(page);
	lock_page_cgroup(pc);
	if (PageCgroupUsed(pc)) {
		memcg = pc->mem_cgroup;
		css_get(&memcg->css);
		 
		if (PageAnon(page))
			SetPageCgroupMigration(pc);
	}
	unlock_page_cgroup(pc);
	 
	if (!memcg)
		return 0;

	*memcgp = memcg;
	ret = __mem_cgroup_try_charge(NULL, gfp_mask, 1, memcgp, false);
	css_put(&memcg->css); 
	if (ret) {
		if (PageAnon(page)) {
			lock_page_cgroup(pc);
			ClearPageCgroupMigration(pc);
			unlock_page_cgroup(pc);
			 
			mem_cgroup_uncharge_page(page);
		}
		 
		return -ENOMEM;
	}
	 
	pc = lookup_page_cgroup(newpage);
	if (PageAnon(page))
		ctype = MEM_CGROUP_CHARGE_TYPE_MAPPED;
	else if (page_is_file_cache(page))
		ctype = MEM_CGROUP_CHARGE_TYPE_CACHE;
	else
		ctype = MEM_CGROUP_CHARGE_TYPE_SHMEM;
	__mem_cgroup_commit_charge(memcg, newpage, 1, pc, ctype, false);
	return ret;
}
