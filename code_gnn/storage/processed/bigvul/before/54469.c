static void copy_huge_page(struct page *dst, struct page *src)
{
	int i;
	int nr_pages;

	if (PageHuge(src)) {
		 
		struct hstate *h = page_hstate(src);
		nr_pages = pages_per_huge_page(h);

		if (unlikely(nr_pages > MAX_ORDER_NR_PAGES)) {
			__copy_gigantic_page(dst, src, nr_pages);
			return;
		}
	} else {
		 
		BUG_ON(!PageTransHuge(src));
		nr_pages = hpage_nr_pages(src);
	}

	for (i = 0; i < nr_pages; i++) {
		cond_resched();
		copy_highpage(dst + i, src + i);
	}
}