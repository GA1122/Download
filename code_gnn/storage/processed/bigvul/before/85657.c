static void __munlock_isolated_page(struct page *page)
{
	 
	if (page_mapcount(page) > 1)
		try_to_munlock(page);

	 
	if (!PageMlocked(page))
		count_vm_event(UNEVICTABLE_PGMUNLOCKED);

	putback_lru_page(page);
}
