static enum bp_state increase_reservation(unsigned long nr_pages)
{
	int rc;
	unsigned long i;
	struct page   *page;

	if (nr_pages > ARRAY_SIZE(frame_list))
		nr_pages = ARRAY_SIZE(frame_list);

	page = list_first_entry_or_null(&ballooned_pages, struct page, lru);
	for (i = 0; i < nr_pages; i++) {
		if (!page) {
			nr_pages = i;
			break;
		}

		frame_list[i] = page_to_xen_pfn(page);
		page = balloon_next_page(page);
	}

	rc = xenmem_reservation_increase(nr_pages, frame_list);
	if (rc <= 0)
		return BP_EAGAIN;

	for (i = 0; i < rc; i++) {
		page = balloon_retrieve(false);
		BUG_ON(page == NULL);

		xenmem_reservation_va_mapping_update(1, &page, &frame_list[i]);

		 
		__ClearPageOffline(page);
		free_reserved_page(page);
	}

	balloon_stats.current_pages += rc;

	return BP_DONE;
}