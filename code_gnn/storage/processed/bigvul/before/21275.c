unsigned long unmap_vmas(struct mmu_gather *tlb,
		struct vm_area_struct *vma, unsigned long start_addr,
		unsigned long end_addr, unsigned long *nr_accounted,
		struct zap_details *details)
{
	unsigned long start = start_addr;
	struct mm_struct *mm = vma->vm_mm;

	mmu_notifier_invalidate_range_start(mm, start_addr, end_addr);
	for ( ; vma && vma->vm_start < end_addr; vma = vma->vm_next) {
		unsigned long end;

		start = max(vma->vm_start, start_addr);
		if (start >= vma->vm_end)
			continue;
		end = min(vma->vm_end, end_addr);
		if (end <= vma->vm_start)
			continue;

		if (vma->vm_flags & VM_ACCOUNT)
			*nr_accounted += (end - start) >> PAGE_SHIFT;

		if (unlikely(is_pfn_mapping(vma)))
			untrack_pfn_vma(vma, 0, 0);

		while (start != end) {
			if (unlikely(is_vm_hugetlb_page(vma))) {
				 
				if (vma->vm_file)
					unmap_hugepage_range(vma, start, end, NULL);

				start = end;
			} else
				start = unmap_page_range(tlb, vma, start, end, details);
		}
	}

	mmu_notifier_invalidate_range_end(mm, start_addr, end_addr);
	return start;	 
}
