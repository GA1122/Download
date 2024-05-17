static void return_unused_surplus_pages(struct hstate *h,
					unsigned long unused_resv_pages)
{
	unsigned long nr_pages;

	 
	h->resv_huge_pages -= unused_resv_pages;

	 
	if (h->order >= MAX_ORDER)
		return;

	nr_pages = min(unused_resv_pages, h->surplus_huge_pages);

	 
	while (nr_pages--) {
		if (!free_pool_huge_page(h, &node_states[N_HIGH_MEMORY], 1))
			break;
	}
}
