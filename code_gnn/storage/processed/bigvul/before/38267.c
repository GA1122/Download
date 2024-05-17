static void __munlock_isolated_page(struct page *page)
{
	int ret = SWAP_AGAIN;

	 
	if (page_mapcount(page) > 1)
		ret = try_to_munlock(page);

	 
	if (ret != SWAP_MLOCK)
		count_vm_event(UNEVICTABLE_PGMUNLOCKED);

	putback_lru_page(page);
}
