static struct page *next_active_pageblock(struct page *page)
{
	 
	BUG_ON(page_to_pfn(page) & (pageblock_nr_pages - 1));

	 
	if (pageblock_free(page)) {
		int order;
		 
		order = page_order(page);
		if ((order < MAX_ORDER) && (order >= pageblock_order))
			return page + (1 << order);
	}

	return page + pageblock_nr_pages;
}
