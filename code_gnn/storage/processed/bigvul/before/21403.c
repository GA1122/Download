static bool swap_count_continued(struct swap_info_struct *si,
				 pgoff_t offset, unsigned char count)
{
	struct page *head;
	struct page *page;
	unsigned char *map;

	head = vmalloc_to_page(si->swap_map + offset);
	if (page_private(head) != SWP_CONTINUED) {
		BUG_ON(count & COUNT_CONTINUED);
		return false;		 
	}

	offset &= ~PAGE_MASK;
	page = list_entry(head->lru.next, struct page, lru);
	map = kmap_atomic(page, KM_USER0) + offset;

	if (count == SWAP_MAP_MAX)	 
		goto init_map;		 

	if (count == (SWAP_MAP_MAX | COUNT_CONTINUED)) {  
		 
		while (*map == (SWAP_CONT_MAX | COUNT_CONTINUED)) {
			kunmap_atomic(map, KM_USER0);
			page = list_entry(page->lru.next, struct page, lru);
			BUG_ON(page == head);
			map = kmap_atomic(page, KM_USER0) + offset;
		}
		if (*map == SWAP_CONT_MAX) {
			kunmap_atomic(map, KM_USER0);
			page = list_entry(page->lru.next, struct page, lru);
			if (page == head)
				return false;	 
			map = kmap_atomic(page, KM_USER0) + offset;
init_map:		*map = 0;		 
		}
		*map += 1;
		kunmap_atomic(map, KM_USER0);
		page = list_entry(page->lru.prev, struct page, lru);
		while (page != head) {
			map = kmap_atomic(page, KM_USER0) + offset;
			*map = COUNT_CONTINUED;
			kunmap_atomic(map, KM_USER0);
			page = list_entry(page->lru.prev, struct page, lru);
		}
		return true;			 

	} else {				 
		 
		BUG_ON(count != COUNT_CONTINUED);
		while (*map == COUNT_CONTINUED) {
			kunmap_atomic(map, KM_USER0);
			page = list_entry(page->lru.next, struct page, lru);
			BUG_ON(page == head);
			map = kmap_atomic(page, KM_USER0) + offset;
		}
		BUG_ON(*map == 0);
		*map -= 1;
		if (*map == 0)
			count = 0;
		kunmap_atomic(map, KM_USER0);
		page = list_entry(page->lru.prev, struct page, lru);
		while (page != head) {
			map = kmap_atomic(page, KM_USER0) + offset;
			*map = SWAP_CONT_MAX | count;
			count = COUNT_CONTINUED;
			kunmap_atomic(map, KM_USER0);
			page = list_entry(page->lru.prev, struct page, lru);
		}
		return count == COUNT_CONTINUED;
	}
}
