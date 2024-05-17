static struct page *dequeue_huge_page_vma(struct hstate *h,
				struct vm_area_struct *vma,
				unsigned long address, int avoid_reserve,
				long chg)
{
	struct page *page;
	struct mempolicy *mpol;
	gfp_t gfp_mask;
	nodemask_t *nodemask;
	int nid;

	 
	if (!vma_has_reserves(vma, chg) &&
			h->free_huge_pages - h->resv_huge_pages == 0)
		goto err;

	 
	if (avoid_reserve && h->free_huge_pages - h->resv_huge_pages == 0)
		goto err;

	gfp_mask = htlb_alloc_mask(h);
	nid = huge_node(vma, address, gfp_mask, &mpol, &nodemask);
	page = dequeue_huge_page_nodemask(h, gfp_mask, nid, nodemask);
	if (page && !avoid_reserve && vma_has_reserves(vma, chg)) {
		SetPagePrivate(page);
		h->resv_huge_pages--;
	}

	mpol_cond_put(mpol);
	return page;

err:
	return NULL;
}
