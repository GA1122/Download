static int queue_pages_pte_range(pmd_t *pmd, unsigned long addr,
			unsigned long end, struct mm_walk *walk)
{
	struct vm_area_struct *vma = walk->vma;
	struct page *page;
	struct queue_pages *qp = walk->private;
	unsigned long flags = qp->flags;
	int nid, ret;
	pte_t *pte;
	spinlock_t *ptl;

	if (pmd_trans_huge(*pmd)) {
		ptl = pmd_lock(walk->mm, pmd);
		if (pmd_trans_huge(*pmd)) {
			page = pmd_page(*pmd);
			if (is_huge_zero_page(page)) {
				spin_unlock(ptl);
				__split_huge_pmd(vma, pmd, addr, false, NULL);
			} else {
				get_page(page);
				spin_unlock(ptl);
				lock_page(page);
				ret = split_huge_page(page);
				unlock_page(page);
				put_page(page);
				if (ret)
					return 0;
			}
		} else {
			spin_unlock(ptl);
		}
	}

	if (pmd_trans_unstable(pmd))
		return 0;
retry:
	pte = pte_offset_map_lock(walk->mm, pmd, addr, &ptl);
	for (; addr != end; pte++, addr += PAGE_SIZE) {
		if (!pte_present(*pte))
			continue;
		page = vm_normal_page(vma, addr, *pte);
		if (!page)
			continue;
		 
		if (PageReserved(page))
			continue;
		nid = page_to_nid(page);
		if (node_isset(nid, *qp->nmask) == !!(flags & MPOL_MF_INVERT))
			continue;
		if (PageTransCompound(page)) {
			get_page(page);
			pte_unmap_unlock(pte, ptl);
			lock_page(page);
			ret = split_huge_page(page);
			unlock_page(page);
			put_page(page);
			 
			if (ret) {
				pte = pte_offset_map_lock(walk->mm, pmd,
						addr, &ptl);
				continue;
			}
			goto retry;
		}

		migrate_page_add(page, qp->pagelist, flags);
	}
	pte_unmap_unlock(pte - 1, ptl);
	cond_resched();
	return 0;
}