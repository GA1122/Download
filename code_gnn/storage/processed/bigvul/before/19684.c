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
		page = alloc_buddy_huge_page(h, NUMA_NO_NODE);
		if (!page) {
			alloc_ok = false;
			break;
		}
		list_add(&page->lru, &surplus_list);
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
		list_del(&page->lru);
		 
		put_page_testzero(page);
		VM_BUG_ON(page_count(page));
		enqueue_huge_page(h, page);
	}
free:
	spin_unlock(&hugetlb_lock);

	 
	if (!list_empty(&surplus_list)) {
		list_for_each_entry_safe(page, tmp, &surplus_list, lru) {
			list_del(&page->lru);
			put_page(page);
		}
	}
	spin_lock(&hugetlb_lock);

	return ret;
}
