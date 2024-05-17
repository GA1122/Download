static struct page *dequeue_huge_page_vma(struct hstate *h,
				struct vm_area_struct *vma,
				unsigned long address, int avoid_reserve)
{
	struct page *page = NULL;
	struct mempolicy *mpol;
	nodemask_t *nodemask;
	struct zonelist *zonelist;
	struct zone *zone;
	struct zoneref *z;
	unsigned int cpuset_mems_cookie;

retry_cpuset:
	cpuset_mems_cookie = get_mems_allowed();
	zonelist = huge_zonelist(vma, address,
					htlb_alloc_mask, &mpol, &nodemask);
	 
	if (!vma_has_reserves(vma) &&
			h->free_huge_pages - h->resv_huge_pages == 0)
		goto err;

	 
	if (avoid_reserve && h->free_huge_pages - h->resv_huge_pages == 0)
		goto err;

	for_each_zone_zonelist_nodemask(zone, z, zonelist,
						MAX_NR_ZONES - 1, nodemask) {
		if (cpuset_zone_allowed_softwall(zone, htlb_alloc_mask)) {
			page = dequeue_huge_page_node(h, zone_to_nid(zone));
			if (page) {
				if (!avoid_reserve)
					decrement_hugepage_resv_vma(h, vma);
				break;
			}
		}
	}

	mpol_cond_put(mpol);
	if (unlikely(!put_mems_allowed(cpuset_mems_cookie) && !page))
		goto retry_cpuset;
	return page;

err:
	mpol_cond_put(mpol);
	return NULL;
}