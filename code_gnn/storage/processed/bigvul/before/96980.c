struct page *alloc_huge_page(struct vm_area_struct *vma,
				    unsigned long addr, int avoid_reserve)
{
	struct hugepage_subpool *spool = subpool_vma(vma);
	struct hstate *h = hstate_vma(vma);
	struct page *page;
	long map_chg, map_commit;
	long gbl_chg;
	int ret, idx;
	struct hugetlb_cgroup *h_cg;

	idx = hstate_index(h);
	 
	map_chg = gbl_chg = vma_needs_reservation(h, vma, addr);
	if (map_chg < 0)
		return ERR_PTR(-ENOMEM);

	 
	if (map_chg || avoid_reserve) {
		gbl_chg = hugepage_subpool_get_pages(spool, 1);
		if (gbl_chg < 0) {
			vma_end_reservation(h, vma, addr);
			return ERR_PTR(-ENOSPC);
		}

		 
		if (avoid_reserve)
			gbl_chg = 1;
	}

	ret = hugetlb_cgroup_charge_cgroup(idx, pages_per_huge_page(h), &h_cg);
	if (ret)
		goto out_subpool_put;

	spin_lock(&hugetlb_lock);
	 
	page = dequeue_huge_page_vma(h, vma, addr, avoid_reserve, gbl_chg);
	if (!page) {
		spin_unlock(&hugetlb_lock);
		page = alloc_buddy_huge_page_with_mpol(h, vma, addr);
		if (!page)
			goto out_uncharge_cgroup;
		if (!avoid_reserve && vma_has_reserves(vma, gbl_chg)) {
			SetPagePrivate(page);
			h->resv_huge_pages--;
		}
		spin_lock(&hugetlb_lock);
		list_move(&page->lru, &h->hugepage_activelist);
		 
	}
	hugetlb_cgroup_commit_charge(idx, pages_per_huge_page(h), h_cg, page);
	spin_unlock(&hugetlb_lock);

	set_page_private(page, (unsigned long)spool);

	map_commit = vma_commit_reservation(h, vma, addr);
	if (unlikely(map_chg > map_commit)) {
		 
		long rsv_adjust;

		rsv_adjust = hugepage_subpool_put_pages(spool, 1);
		hugetlb_acct_memory(h, -rsv_adjust);
	}
	return page;

out_uncharge_cgroup:
	hugetlb_cgroup_uncharge_cgroup(idx, pages_per_huge_page(h), h_cg);
out_subpool_put:
	if (map_chg || avoid_reserve)
		hugepage_subpool_put_pages(spool, 1);
	vma_end_reservation(h, vma, addr);
	return ERR_PTR(-ENOSPC);
}
