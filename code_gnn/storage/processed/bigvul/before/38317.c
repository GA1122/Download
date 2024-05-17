void page_remove_rmap(struct page *page)
{
	bool anon = PageAnon(page);
	bool locked;
	unsigned long flags;

	 
	if (!anon)
		mem_cgroup_begin_update_page_stat(page, &locked, &flags);

	 
	if (!atomic_add_negative(-1, &page->_mapcount))
		goto out;

	 
	if (unlikely(PageHuge(page)))
		goto out;
	if (anon) {
		mem_cgroup_uncharge_page(page);
		if (PageTransHuge(page))
			__dec_zone_page_state(page,
					      NR_ANON_TRANSPARENT_HUGEPAGES);
		__mod_zone_page_state(page_zone(page), NR_ANON_PAGES,
				-hpage_nr_pages(page));
	} else {
		__dec_zone_page_state(page, NR_FILE_MAPPED);
		mem_cgroup_dec_page_stat(page, MEM_CGROUP_STAT_FILE_MAPPED);
		mem_cgroup_end_update_page_stat(page, &locked, &flags);
	}
	if (unlikely(PageMlocked(page)))
		clear_page_mlock(page);
	 
	return;
out:
	if (!anon)
		mem_cgroup_end_update_page_stat(page, &locked, &flags);
}
