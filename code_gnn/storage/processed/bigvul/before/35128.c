static void release_pte_page(struct page *page)
{
	 
	dec_zone_page_state(page, NR_ISOLATED_ANON + 0);
	unlock_page(page);
	putback_lru_page(page);
}
