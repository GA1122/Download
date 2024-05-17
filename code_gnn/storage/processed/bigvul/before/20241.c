static void truncate_huge_page(struct page *page)
{
	cancel_dirty_page(page,  0);
	ClearPageUptodate(page);
	delete_from_page_cache(page);
}
