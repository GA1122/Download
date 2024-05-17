static int numamigrate_isolate_page(pg_data_t *pgdat, struct page *page)
{
	int page_lru;

	VM_BUG_ON_PAGE(compound_order(page) && !PageTransHuge(page), page);

	 
	if (!migrate_balanced_pgdat(pgdat, 1UL << compound_order(page)))
		return 0;

	if (isolate_lru_page(page))
		return 0;

	 
	if (PageTransHuge(page) && page_count(page) != 3) {
		putback_lru_page(page);
		return 0;
	}

	page_lru = page_is_file_cache(page);
	mod_zone_page_state(page_zone(page), NR_ISOLATED_ANON + page_lru,
				hpage_nr_pages(page));

	 
	put_page(page);
	return 1;
}
