static long check_and_migrate_cma_pages(unsigned long start, long nr_pages,
					unsigned int gup_flags,
					struct page **pages,
					struct vm_area_struct **vmas)
{
	long i;
	bool drain_allow = true;
	bool migrate_allow = true;
	LIST_HEAD(cma_page_list);

check_again:
	for (i = 0; i < nr_pages; i++) {
		 
		if (is_migrate_cma_page(pages[i])) {

			struct page *head = compound_head(pages[i]);

			if (PageHuge(head)) {
				isolate_huge_page(head, &cma_page_list);
			} else {
				if (!PageLRU(head) && drain_allow) {
					lru_add_drain_all();
					drain_allow = false;
				}

				if (!isolate_lru_page(head)) {
					list_add_tail(&head->lru, &cma_page_list);
					mod_node_page_state(page_pgdat(head),
							    NR_ISOLATED_ANON +
							    page_is_file_cache(head),
							    hpage_nr_pages(head));
				}
			}
		}
	}

	if (!list_empty(&cma_page_list)) {
		 
		for (i = 0; i < nr_pages; i++)
			put_page(pages[i]);

		if (migrate_pages(&cma_page_list, new_non_cma_page,
				  NULL, 0, MIGRATE_SYNC, MR_CONTIG_RANGE)) {
			 
			migrate_allow = false;

			if (!list_empty(&cma_page_list))
				putback_movable_pages(&cma_page_list);
		}
		 
		nr_pages = get_user_pages(start, nr_pages, gup_flags, pages, vmas);
		if ((nr_pages > 0) && migrate_allow) {
			drain_allow = true;
			goto check_again;
		}
	}

	return nr_pages;
}
