void munlock_vma_pages_range(struct vm_area_struct *vma,
			     unsigned long start, unsigned long end)
{
	vma->vm_flags &= ~VM_LOCKED;

	while (start < end) {
		struct page *page = NULL;
		unsigned int page_mask;
		unsigned long page_increm;
		struct pagevec pvec;
		struct zone *zone;
		int zoneid;

		pagevec_init(&pvec, 0);
		 
		page = follow_page_mask(vma, start, FOLL_GET | FOLL_DUMP,
				&page_mask);

		if (page && !IS_ERR(page)) {
			if (PageTransHuge(page)) {
				lock_page(page);
				 
				page_mask = munlock_vma_page(page);
				unlock_page(page);
				put_page(page);  
			} else {
				 
				pagevec_add(&pvec, page);
				zone = page_zone(page);
				zoneid = page_zone_id(page);

				 
				start = __munlock_pagevec_fill(&pvec, vma,
						zoneid, start, end);
				__munlock_pagevec(&pvec, zone);
				goto next;
			}
		}
		 
		VM_BUG_ON((start >> PAGE_SHIFT) & page_mask);
		page_increm = 1 + page_mask;
		start += page_increm * PAGE_SIZE;
next:
		cond_resched();
	}
}
