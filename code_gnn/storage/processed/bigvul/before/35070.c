static int __collapse_huge_page_isolate(struct vm_area_struct *vma,
					unsigned long address,
					pte_t *pte)
{
	struct page *page;
	pte_t *_pte;
	int referenced = 0, isolated = 0, none = 0;
	for (_pte = pte; _pte < pte+HPAGE_PMD_NR;
	     _pte++, address += PAGE_SIZE) {
		pte_t pteval = *_pte;
		if (pte_none(pteval)) {
			if (++none <= khugepaged_max_ptes_none)
				continue;
			else {
				release_pte_pages(pte, _pte);
				goto out;
			}
		}
		if (!pte_present(pteval) || !pte_write(pteval)) {
			release_pte_pages(pte, _pte);
			goto out;
		}
		page = vm_normal_page(vma, address, pteval);
		if (unlikely(!page)) {
			release_pte_pages(pte, _pte);
			goto out;
		}
		VM_BUG_ON(PageCompound(page));
		BUG_ON(!PageAnon(page));
		VM_BUG_ON(!PageSwapBacked(page));

		 
		if (page_count(page) != 1) {
			release_pte_pages(pte, _pte);
			goto out;
		}
		 
		if (!trylock_page(page)) {
			release_pte_pages(pte, _pte);
			goto out;
		}
		 
		if (isolate_lru_page(page)) {
			unlock_page(page);
			release_pte_pages(pte, _pte);
			goto out;
		}
		 
		inc_zone_page_state(page, NR_ISOLATED_ANON + 0);
		VM_BUG_ON(!PageLocked(page));
		VM_BUG_ON(PageLRU(page));

		 
		if (pte_young(pteval) || PageReferenced(page) ||
		    mmu_notifier_test_young(vma->vm_mm, address))
			referenced = 1;
	}
	if (unlikely(!referenced))
		release_all_pte_pages(pte);
	else
		isolated = 1;
out:
	return isolated;
}
