static void prep_compound_gigantic_page(struct page *page, unsigned long order)
{
	int i;
	int nr_pages = 1 << order;
	struct page *p = page + 1;

	 
	set_compound_order(page, order);
	__SetPageHead(page);
	for (i = 1; i < nr_pages; i++, p = mem_map_next(p, page, i)) {
		__SetPageTail(p);
		set_page_count(p, 0);
		p->first_page = page;
	}
}