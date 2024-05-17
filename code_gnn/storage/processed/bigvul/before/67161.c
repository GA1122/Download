alloc_pages_vma(gfp_t gfp, int order, struct vm_area_struct *vma,
		unsigned long addr, int node, bool hugepage)
{
	struct mempolicy *pol;
	struct page *page;
	unsigned int cpuset_mems_cookie;
	struct zonelist *zl;
	nodemask_t *nmask;

retry_cpuset:
	pol = get_vma_policy(vma, addr);
	cpuset_mems_cookie = read_mems_allowed_begin();

	if (pol->mode == MPOL_INTERLEAVE) {
		unsigned nid;

		nid = interleave_nid(pol, vma, addr, PAGE_SHIFT + order);
		mpol_cond_put(pol);
		page = alloc_page_interleave(gfp, order, nid);
		goto out;
	}

	if (unlikely(IS_ENABLED(CONFIG_TRANSPARENT_HUGEPAGE) && hugepage)) {
		int hpage_node = node;

		 
		if (pol->mode == MPOL_PREFERRED &&
						!(pol->flags & MPOL_F_LOCAL))
			hpage_node = pol->v.preferred_node;

		nmask = policy_nodemask(gfp, pol);
		if (!nmask || node_isset(hpage_node, *nmask)) {
			mpol_cond_put(pol);
			page = __alloc_pages_node(hpage_node,
						gfp | __GFP_THISNODE, order);
			goto out;
		}
	}

	nmask = policy_nodemask(gfp, pol);
	zl = policy_zonelist(gfp, pol, node);
	page = __alloc_pages_nodemask(gfp, order, zl, nmask);
	mpol_cond_put(pol);
out:
	if (unlikely(!page && read_mems_allowed_retry(cpuset_mems_cookie)))
		goto retry_cpuset;
	return page;
}
