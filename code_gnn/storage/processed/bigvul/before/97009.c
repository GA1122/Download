void unmap_hugepage_range(struct vm_area_struct *vma, unsigned long start,
			  unsigned long end, struct page *ref_page)
{
	struct mm_struct *mm;
	struct mmu_gather tlb;
	unsigned long tlb_start = start;
	unsigned long tlb_end = end;

	 
	adjust_range_if_pmd_sharing_possible(vma, &tlb_start, &tlb_end);

	mm = vma->vm_mm;

	tlb_gather_mmu(&tlb, mm, tlb_start, tlb_end);
	__unmap_hugepage_range(&tlb, vma, start, end, ref_page);
	tlb_finish_mmu(&tlb, tlb_start, tlb_end);
}
