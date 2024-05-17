void mem_cgroup_uncharge_page(struct page *page)
{
	 
	if (page_mapped(page))
		return;
	VM_BUG_ON(page->mapping && !PageAnon(page));
	__mem_cgroup_uncharge_common(page, MEM_CGROUP_CHARGE_TYPE_MAPPED);
}