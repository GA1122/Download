static ICE_noinline int unmap_and_move(new_page_t get_new_page,
				   free_page_t put_new_page,
				   unsigned long private, struct page *page,
				   int force, enum migrate_mode mode,
				   enum migrate_reason reason)
{
	int rc = MIGRATEPAGE_SUCCESS;
	int *result = NULL;
	struct page *newpage;

	newpage = get_new_page(page, private, &result);
	if (!newpage)
		return -ENOMEM;

	if (page_count(page) == 1) {
		 
		goto out;
	}

	if (unlikely(PageTransHuge(page)))
		if (unlikely(split_huge_page(page)))
			goto out;

	rc = __unmap_and_move(page, newpage, force, mode);
	if (rc == MIGRATEPAGE_SUCCESS)
		put_new_page = NULL;

out:
	if (rc != -EAGAIN) {
		 
		list_del(&page->lru);
		dec_zone_page_state(page, NR_ISOLATED_ANON +
				page_is_file_cache(page));
		 
		if (reason == MR_MEMORY_FAILURE) {
			put_page(page);
			if (!test_set_page_hwpoison(page))
				num_poisoned_pages_inc();
		} else
			putback_lru_page(page);
	}

	 
	if (put_new_page)
		put_new_page(newpage, private);
	else if (unlikely(__is_movable_balloon_page(newpage))) {
		 
		put_page(newpage);
	} else
		putback_lru_page(newpage);

	if (result) {
		if (rc)
			*result = rc;
		else
			*result = page_to_nid(newpage);
	}
	return rc;
}