void free_huge_page(struct page *page)
{
	 
	struct hstate *h = page_hstate(page);
	int nid = page_to_nid(page);
	struct hugepage_subpool *spool =
		(struct hugepage_subpool *)page_private(page);
	bool restore_reserve;

	set_page_private(page, 0);
	page->mapping = NULL;
	VM_BUG_ON_PAGE(page_count(page), page);
	VM_BUG_ON_PAGE(page_mapcount(page), page);
	restore_reserve = PagePrivate(page);
	ClearPagePrivate(page);

	 
	if (hugepage_subpool_put_pages(spool, 1) == 0)
		restore_reserve = true;

	spin_lock(&hugetlb_lock);
	clear_page_huge_active(page);
	hugetlb_cgroup_uncharge_page(hstate_index(h),
				     pages_per_huge_page(h), page);
	if (restore_reserve)
		h->resv_huge_pages++;

	if (h->surplus_huge_pages_node[nid]) {
		 
		list_del(&page->lru);
		update_and_free_page(h, page);
		h->surplus_huge_pages--;
		h->surplus_huge_pages_node[nid]--;
	} else {
		arch_clear_hugepage_flags(page);
		enqueue_huge_page(h, page);
	}
	spin_unlock(&hugetlb_lock);
}
