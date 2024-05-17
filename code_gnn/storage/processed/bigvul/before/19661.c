void __unmap_hugepage_range(struct vm_area_struct *vma, unsigned long start,
			    unsigned long end, struct page *ref_page)
{
	struct mm_struct *mm = vma->vm_mm;
	unsigned long address;
	pte_t *ptep;
	pte_t pte;
	struct page *page;
	struct page *tmp;
	struct hstate *h = hstate_vma(vma);
	unsigned long sz = huge_page_size(h);

	 
	LIST_HEAD(page_list);

	WARN_ON(!is_vm_hugetlb_page(vma));
	BUG_ON(start & ~huge_page_mask(h));
	BUG_ON(end & ~huge_page_mask(h));

	mmu_notifier_invalidate_range_start(mm, start, end);
	spin_lock(&mm->page_table_lock);
	for (address = start; address < end; address += sz) {
		ptep = huge_pte_offset(mm, address);
		if (!ptep)
			continue;

		if (huge_pmd_unshare(mm, &address, ptep))
			continue;

		pte = huge_ptep_get(ptep);
		if (huge_pte_none(pte))
			continue;

		 
		if (unlikely(is_hugetlb_entry_hwpoisoned(pte)))
			continue;

		page = pte_page(pte);
		 
		if (ref_page) {
			if (page != ref_page)
				continue;

			 
			set_vma_resv_flags(vma, HPAGE_RESV_UNMAPPED);
		}

		pte = huge_ptep_get_and_clear(mm, address, ptep);
		if (pte_dirty(pte))
			set_page_dirty(page);
		list_add(&page->lru, &page_list);

		 
		if (ref_page)
			break;
	}
	flush_tlb_range(vma, start, end);
	spin_unlock(&mm->page_table_lock);
	mmu_notifier_invalidate_range_end(mm, start, end);
	list_for_each_entry_safe(page, tmp, &page_list, lru) {
		page_remove_rmap(page);
		list_del(&page->lru);
		put_page(page);
	}
}