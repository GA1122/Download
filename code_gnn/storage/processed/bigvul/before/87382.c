int alloc_xenballooned_pages(int nr_pages, struct page **pages)
{
	int pgno = 0;
	struct page *page;
	int ret;

	mutex_lock(&balloon_mutex);

	balloon_stats.target_unpopulated += nr_pages;

	while (pgno < nr_pages) {
		page = balloon_retrieve(true);
		if (page) {
			__ClearPageOffline(page);
			pages[pgno++] = page;
#ifdef CONFIG_XEN_HAVE_PVMMU
			 
			BUILD_BUG_ON(XEN_PAGE_SIZE != PAGE_SIZE);

			if (!xen_feature(XENFEAT_auto_translated_physmap)) {
				ret = xen_alloc_p2m_entry(page_to_pfn(page));
				if (ret < 0)
					goto out_undo;
			}
#endif
		} else {
			ret = add_ballooned_pages(nr_pages - pgno);
			if (ret < 0)
				goto out_undo;
		}
	}
	mutex_unlock(&balloon_mutex);
	return 0;
 out_undo:
	mutex_unlock(&balloon_mutex);
	free_xenballooned_pages(pgno, pages);
	return ret;
}