int do_huge_pmd_anonymous_page(struct mm_struct *mm, struct vm_area_struct *vma,
			       unsigned long address, pmd_t *pmd,
			       unsigned int flags)
{
	struct page *page;
	unsigned long haddr = address & HPAGE_PMD_MASK;
	pte_t *pte;

	if (haddr >= vma->vm_start && haddr + HPAGE_PMD_SIZE <= vma->vm_end) {
		if (unlikely(anon_vma_prepare(vma)))
			return VM_FAULT_OOM;
		if (unlikely(khugepaged_enter(vma)))
			return VM_FAULT_OOM;
		page = alloc_hugepage_vma(transparent_hugepage_defrag(vma),
					  vma, haddr, numa_node_id(), 0);
		if (unlikely(!page)) {
			count_vm_event(THP_FAULT_FALLBACK);
			goto out;
		}
		count_vm_event(THP_FAULT_ALLOC);
		if (unlikely(mem_cgroup_newpage_charge(page, mm, GFP_KERNEL))) {
			put_page(page);
			goto out;
		}

		return __do_huge_pmd_anonymous_page(mm, vma, haddr, pmd, page);
	}
out:
	 
	if (unlikely(__pte_alloc(mm, vma, pmd, address)))
		return VM_FAULT_OOM;
	 
	if (unlikely(pmd_trans_huge(*pmd)))
		return 0;
	 
	pte = pte_offset_map(pmd, address);
	return handle_pte_fault(mm, vma, address, pte, pmd, flags);
}
