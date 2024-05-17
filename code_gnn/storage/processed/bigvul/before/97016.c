static struct page *alloc_migrate_huge_page(struct hstate *h, gfp_t gfp_mask,
		int nid, nodemask_t *nmask)
{
	struct page *page;

	if (hstate_is_gigantic(h))
		return NULL;

	page = alloc_fresh_huge_page(h, gfp_mask, nid, nmask);
	if (!page)
		return NULL;

	 
	SetPageHugeTemporary(page);

	return page;
}
