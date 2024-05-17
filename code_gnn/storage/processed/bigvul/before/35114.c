static int khugepaged_scan_pmd(struct mm_struct *mm,
			       struct vm_area_struct *vma,
			       unsigned long address,
			       struct page **hpage)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte, *_pte;
	int ret = 0, referenced = 0, none = 0;
	struct page *page;
	unsigned long _address;
	spinlock_t *ptl;
	int node = -1;

	VM_BUG_ON(address & ~HPAGE_PMD_MASK);

	pgd = pgd_offset(mm, address);
	if (!pgd_present(*pgd))
		goto out;

	pud = pud_offset(pgd, address);
	if (!pud_present(*pud))
		goto out;

	pmd = pmd_offset(pud, address);
	if (!pmd_present(*pmd) || pmd_trans_huge(*pmd))
		goto out;

	pte = pte_offset_map_lock(mm, pmd, address, &ptl);
	for (_address = address, _pte = pte; _pte < pte+HPAGE_PMD_NR;
	     _pte++, _address += PAGE_SIZE) {
		pte_t pteval = *_pte;
		if (pte_none(pteval)) {
			if (++none <= khugepaged_max_ptes_none)
				continue;
			else
				goto out_unmap;
		}
		if (!pte_present(pteval) || !pte_write(pteval))
			goto out_unmap;
		page = vm_normal_page(vma, _address, pteval);
		if (unlikely(!page))
			goto out_unmap;
		 
		if (node == -1)
			node = page_to_nid(page);
		VM_BUG_ON(PageCompound(page));
		if (!PageLRU(page) || PageLocked(page) || !PageAnon(page))
			goto out_unmap;
		 
		if (page_count(page) != 1)
			goto out_unmap;
		if (pte_young(pteval) || PageReferenced(page) ||
		    mmu_notifier_test_young(vma->vm_mm, address))
			referenced = 1;
	}
	if (referenced)
		ret = 1;
out_unmap:
	pte_unmap_unlock(pte, ptl);
	if (ret)
		 
		collapse_huge_page(mm, address, hpage, vma, node);
out:
	return ret;
}