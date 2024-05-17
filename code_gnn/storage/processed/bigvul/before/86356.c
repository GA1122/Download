static int gather_surplus_pages(struct hstate *h, int delta)
{
	struct list_head surplus_list;
	struct page *page, *tmp;
	int ret, i;
	int needed, allocated;
	bool alloc_ok = true;

	needed = (h->resv_huge_pages + delta) - h->free_huge_pages;
	if (needed <= 0) {
		h->resv_huge_pages += delta;
		return 0;
	}

	allocated = 0;
	INIT_LIST_HEAD(&surplus_list);

	ret = -ENOMEM;
retry:
	spin_unlock(&hugetlb_lock);
	for (i = 0; i < needed; i++) {
		page = __alloc_buddy_huge_page(h, htlb_alloc_mask(h),
				NUMA_NO_NODE, NULL);
		if (!page) {
			alloc_ok = false;
			break;
		}
		list_add(&page->lru, &surplus_list);
		cond_resched();
	}
	allocated += i;

	 
	spin_lock(&hugetlb_lock);
	needed = (h->resv_huge_pages + delta) -
			(h->free_huge_pages + allocated);
	if (needed > 0) {
		if (alloc_ok)
			goto retry;
		 
		goto free;
	}
	 
	needed += allocated;
	h->resv_huge_pages += delta;
	ret = 0;

	 
	list_for_each_entry_safe(page, tmp, &surplus_list, lru) {
		if ((--needed) < 0)
			break;
		 
		put_page_testzero(page);
		VM_BUG_ON_PAGE(page_count(page), page);
		enqueue_huge_page(h, page);
	}
free:
	spin_unlock(&hugetlb_lock);

	 
	list_for_each_entry_safe(page, tmp, &surplus_list, lru)
		put_page(page);
	spin_lock(&hugetlb_lock);

	return ret;
}
