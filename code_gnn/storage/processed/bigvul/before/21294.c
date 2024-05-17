alloc_pages_vma(gfp_t gfp, int order, struct vm_area_struct *vma,
		unsigned long addr, int node)
{
	struct mempolicy *pol = get_vma_policy(current, vma, addr);
	struct zonelist *zl;
	struct page *page;

	get_mems_allowed();
	if (unlikely(pol->mode == MPOL_INTERLEAVE)) {
		unsigned nid;

		nid = interleave_nid(pol, vma, addr, PAGE_SHIFT + order);
		mpol_cond_put(pol);
		page = alloc_page_interleave(gfp, order, nid);
		put_mems_allowed();
		return page;
	}
	zl = policy_zonelist(gfp, pol, node);
	if (unlikely(mpol_needs_cond_ref(pol))) {
		 
		struct page *page =  __alloc_pages_nodemask(gfp, order,
						zl, policy_nodemask(gfp, pol));
		__mpol_put(pol);
		put_mems_allowed();
		return page;
	}
	 
	page = __alloc_pages_nodemask(gfp, order, zl,
				      policy_nodemask(gfp, pol));
	put_mems_allowed();
	return page;
}
