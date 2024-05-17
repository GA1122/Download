static int wp_pfn_shared(struct mm_struct *mm,
			struct vm_area_struct *vma, unsigned long address,
			pte_t *page_table, spinlock_t *ptl, pte_t orig_pte,
			pmd_t *pmd)
{
	if (vma->vm_ops && vma->vm_ops->pfn_mkwrite) {
		struct vm_fault vmf = {
			.page = NULL,
			.pgoff = linear_page_index(vma, address),
			.virtual_address = (void __user *)(address & PAGE_MASK),
			.flags = FAULT_FLAG_WRITE | FAULT_FLAG_MKWRITE,
		};
		int ret;

		pte_unmap_unlock(page_table, ptl);
		ret = vma->vm_ops->pfn_mkwrite(vma, &vmf);
		if (ret & VM_FAULT_ERROR)
			return ret;
		page_table = pte_offset_map_lock(mm, pmd, address, &ptl);
		 
		if (!pte_same(*page_table, orig_pte)) {
			pte_unmap_unlock(page_table, ptl);
			return 0;
		}
	}
	return wp_page_reuse(mm, vma, address, page_table, ptl, orig_pte,
			     NULL, 0, 0);
}