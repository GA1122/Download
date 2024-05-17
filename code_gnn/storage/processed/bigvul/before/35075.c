static int __split_huge_page_map(struct page *page,
				 struct vm_area_struct *vma,
				 unsigned long address)
{
	struct mm_struct *mm = vma->vm_mm;
	pmd_t *pmd, _pmd;
	int ret = 0, i;
	pgtable_t pgtable;
	unsigned long haddr;

	spin_lock(&mm->page_table_lock);
	pmd = page_check_address_pmd(page, mm, address,
				     PAGE_CHECK_ADDRESS_PMD_SPLITTING_FLAG);
	if (pmd) {
		pgtable = get_pmd_huge_pte(mm);
		pmd_populate(mm, &_pmd, pgtable);

		for (i = 0, haddr = address; i < HPAGE_PMD_NR;
		     i++, haddr += PAGE_SIZE) {
			pte_t *pte, entry;
			BUG_ON(PageCompound(page+i));
			entry = mk_pte(page + i, vma->vm_page_prot);
			entry = maybe_mkwrite(pte_mkdirty(entry), vma);
			if (!pmd_write(*pmd))
				entry = pte_wrprotect(entry);
			else
				BUG_ON(page_mapcount(page) != 1);
			if (!pmd_young(*pmd))
				entry = pte_mkold(entry);
			pte = pte_offset_map(&_pmd, haddr);
			BUG_ON(!pte_none(*pte));
			set_pte_at(mm, haddr, pte, entry);
			pte_unmap(pte);
		}

		mm->nr_ptes++;
		smp_wmb();  
		 
		set_pmd_at(mm, address, pmd, pmd_mknotpresent(*pmd));
		flush_tlb_range(vma, address, address + HPAGE_PMD_SIZE);
		pmd_populate(mm, pmd, pgtable);
		ret = 1;
	}
	spin_unlock(&mm->page_table_lock);

	return ret;
}
