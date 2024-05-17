static int queue_pages_hugetlb(pte_t *pte, unsigned long hmask,
			       unsigned long addr, unsigned long end,
			       struct mm_walk *walk)
{
#ifdef CONFIG_HUGETLB_PAGE
	struct queue_pages *qp = walk->private;
	unsigned long flags = qp->flags;
	int nid;
	struct page *page;
	spinlock_t *ptl;
	pte_t entry;

	ptl = huge_pte_lock(hstate_vma(walk->vma), walk->mm, pte);
	entry = huge_ptep_get(pte);
	if (!pte_present(entry))
		goto unlock;
	page = pte_page(entry);
	nid = page_to_nid(page);
	if (node_isset(nid, *qp->nmask) == !!(flags & MPOL_MF_INVERT))
		goto unlock;
	 
	if (flags & (MPOL_MF_MOVE_ALL) ||
	    (flags & MPOL_MF_MOVE && page_mapcount(page) == 1))
		isolate_huge_page(page, qp->pagelist);
unlock:
	spin_unlock(ptl);
#else
	BUG();
#endif
	return 0;
}