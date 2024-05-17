void munlock_vma_pages_range(struct vm_area_struct *vma,
			     unsigned long start, unsigned long end)
{
	vma->vm_flags &= VM_LOCKED_CLEAR_MASK;

	while (start < end) {
		struct page *page;
		unsigned int page_mask = 0;
		unsigned long page_increm;
		struct pagevec pvec;
		struct zone *zone;
		int zoneid;

		pagevec_init(&pvec, 0);
		 
		page = follow_page(vma, start, FOLL_GET | FOLL_DUMP);

		if (page && !IS_ERR(page)) {
			if (PageTransTail(page)) {
				VM_BUG_ON_PAGE(PageMlocked(page), page);
				put_page(page);  
			} else if (PageTransHuge(page)) {
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
		page_increm = 1 + page_mask;
		start += page_increm * PAGE_SIZE;
next:
		cond_resched();
	}
}
