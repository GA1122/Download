static int do_read_fault(struct mm_struct *mm, struct vm_area_struct *vma,
		unsigned long address, pmd_t *pmd,
		pgoff_t pgoff, unsigned int flags, pte_t orig_pte)
{
	struct page *fault_page;
	spinlock_t *ptl;
	pte_t *pte;
	int ret = 0;

	 
	if (vma->vm_ops->map_pages && fault_around_bytes >> PAGE_SHIFT > 1) {
		pte = pte_offset_map_lock(mm, pmd, address, &ptl);
		do_fault_around(vma, address, pte, pgoff, flags);
		if (!pte_same(*pte, orig_pte))
			goto unlock_out;
		pte_unmap_unlock(pte, ptl);
	}

	ret = __do_fault(vma, address, pgoff, flags, NULL, &fault_page);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
		return ret;

	pte = pte_offset_map_lock(mm, pmd, address, &ptl);
	if (unlikely(!pte_same(*pte, orig_pte))) {
		pte_unmap_unlock(pte, ptl);
		unlock_page(fault_page);
		page_cache_release(fault_page);
		return ret;
	}
	do_set_pte(vma, address, fault_page, pte, false, false);
	unlock_page(fault_page);
unlock_out:
	pte_unmap_unlock(pte, ptl);
	return ret;
}