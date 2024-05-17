static struct page *__alloc_buddy_huge_page(struct hstate *h, gfp_t gfp_mask,
		int nid, nodemask_t *nmask)
{
	struct page *page;
	unsigned int r_nid;

	if (hstate_is_gigantic(h))
		return NULL;

	 
	spin_lock(&hugetlb_lock);
	if (h->surplus_huge_pages >= h->nr_overcommit_huge_pages) {
		spin_unlock(&hugetlb_lock);
		return NULL;
	} else {
		h->nr_huge_pages++;
		h->surplus_huge_pages++;
	}
	spin_unlock(&hugetlb_lock);

	page = __hugetlb_alloc_buddy_huge_page(h, gfp_mask, nid, nmask);

	spin_lock(&hugetlb_lock);
	if (page) {
		INIT_LIST_HEAD(&page->lru);
		r_nid = page_to_nid(page);
		set_compound_page_dtor(page, HUGETLB_PAGE_DTOR);
		set_hugetlb_cgroup(page, NULL);
		 
		h->nr_huge_pages_node[r_nid]++;
		h->surplus_huge_pages_node[r_nid]++;
		__count_vm_event(HTLB_BUDDY_PGALLOC);
	} else {
		h->nr_huge_pages--;
		h->surplus_huge_pages--;
		__count_vm_event(HTLB_BUDDY_PGALLOC_FAIL);
	}
	spin_unlock(&hugetlb_lock);

	return page;
}
