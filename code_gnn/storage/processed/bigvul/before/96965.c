static struct page *new_non_cma_page(struct page *page, unsigned long private)
{
	 
	int nid = page_to_nid(page);
	 
	gfp_t gfp_mask = GFP_USER | __GFP_NOWARN;

	if (PageHighMem(page))
		gfp_mask |= __GFP_HIGHMEM;

#ifdef CONFIG_HUGETLB_PAGE
	if (PageHuge(page)) {
		struct hstate *h = page_hstate(page);
		 
		return alloc_migrate_huge_page(h, gfp_mask, nid, NULL);
	}
#endif
	if (PageTransHuge(page)) {
		struct page *thp;
		 
		gfp_t thp_gfpmask = GFP_TRANSHUGE | __GFP_NOWARN;

		 
		thp_gfpmask &= ~__GFP_MOVABLE;
		thp = __alloc_pages_node(nid, thp_gfpmask, HPAGE_PMD_ORDER);
		if (!thp)
			return NULL;
		prep_transhuge_page(thp);
		return thp;
	}

	return __alloc_pages_node(nid, gfp_mask, 0);
}
