static struct page_cgroup *lookup_page_cgroup_used(struct page *page)
{
	struct page_cgroup *pc;

	pc = lookup_page_cgroup(page);
	 
	if (likely(pc) && PageCgroupUsed(pc))
		return pc;
	return NULL;
}
