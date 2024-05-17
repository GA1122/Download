int add_swap_count_continuation(swp_entry_t entry, gfp_t gfp_mask)
{
	struct swap_info_struct *si;
	struct page *head;
	struct page *page;
	struct page *list_page;
	pgoff_t offset;
	unsigned char count;

	 
	page = alloc_page(gfp_mask | __GFP_HIGHMEM);

	si = swap_info_get(entry);
	if (!si) {
		 
		goto outer;
	}

	offset = swp_offset(entry);
	count = si->swap_map[offset] & ~SWAP_HAS_CACHE;

	if ((count & ~COUNT_CONTINUED) != SWAP_MAP_MAX) {
		 
		goto out;
	}

	if (!page) {
		spin_unlock(&swap_lock);
		return -ENOMEM;
	}

	 
	head = vmalloc_to_page(si->swap_map + offset);
	offset &= ~PAGE_MASK;

	 
	if (!page_private(head)) {
		BUG_ON(count & COUNT_CONTINUED);
		INIT_LIST_HEAD(&head->lru);
		set_page_private(head, SWP_CONTINUED);
		si->flags |= SWP_CONTINUED;
	}

	list_for_each_entry(list_page, &head->lru, lru) {
		unsigned char *map;

		 
		if (!(count & COUNT_CONTINUED))
			goto out;

		map = kmap_atomic(list_page, KM_USER0) + offset;
		count = *map;
		kunmap_atomic(map, KM_USER0);

		 
		if ((count & ~COUNT_CONTINUED) != SWAP_CONT_MAX)
			goto out;
	}

	list_add_tail(&page->lru, &head->lru);
	page = NULL;			 
out:
	spin_unlock(&swap_lock);
outer:
	if (page)
		__free_page(page);
	return 0;
}
