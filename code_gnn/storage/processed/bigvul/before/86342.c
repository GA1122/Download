int dissolve_free_huge_page(struct page *page)
{
	int rc = 0;

	spin_lock(&hugetlb_lock);
	if (PageHuge(page) && !page_count(page)) {
		struct page *head = compound_head(page);
		struct hstate *h = page_hstate(head);
		int nid = page_to_nid(head);
		if (h->free_huge_pages - h->resv_huge_pages == 0) {
			rc = -EBUSY;
			goto out;
		}
		 
		if (PageHWPoison(head) && page != head) {
			SetPageHWPoison(page);
			ClearPageHWPoison(head);
		}
		list_del(&head->lru);
		h->free_huge_pages--;
		h->free_huge_pages_node[nid]--;
		h->max_huge_pages--;
		update_and_free_page(h, head);
	}
out:
	spin_unlock(&hugetlb_lock);
	return rc;
}
