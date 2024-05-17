int f2fs_release_page(struct page *page, gfp_t wait)
{
	 
	if (PageDirty(page))
		return 0;

	 
	if (IS_ATOMIC_WRITTEN_PAGE(page))
		return 0;

	set_page_private(page, 0);
	ClearPagePrivate(page);
	return 1;
}
