int try_to_free_swap(struct page *page)
{
	VM_BUG_ON(!PageLocked(page));

	if (!PageSwapCache(page))
		return 0;
	if (PageWriteback(page))
		return 0;
	if (page_swapcount(page))
		return 0;

	 
	if (pm_suspended_storage())
		return 0;

	delete_from_swap_cache(page);
	SetPageDirty(page);
	return 1;
}
