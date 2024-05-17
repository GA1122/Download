static struct page *alloc_huge_page(struct vm_area_struct *vma,
				    unsigned long addr, int avoid_reserve)
{
	struct hugepage_subpool *spool = subpool_vma(vma);
	struct hstate *h = hstate_vma(vma);
	struct page *page;
	long chg;

	 
	chg = vma_needs_reservation(h, vma, addr);
	if (chg < 0)
		return ERR_PTR(-VM_FAULT_OOM);
	if (chg)
		if (hugepage_subpool_get_pages(spool, chg))
			return ERR_PTR(-VM_FAULT_SIGBUS);

	spin_lock(&hugetlb_lock);
	page = dequeue_huge_page_vma(h, vma, addr, avoid_reserve);
	spin_unlock(&hugetlb_lock);

	if (!page) {
		page = alloc_buddy_huge_page(h, NUMA_NO_NODE);
		if (!page) {
			hugepage_subpool_put_pages(spool, chg);
			return ERR_PTR(-VM_FAULT_SIGBUS);
		}
	}

	set_page_private(page, (unsigned long)spool);

	vma_commit_reservation(h, vma, addr);

	return page;
}
