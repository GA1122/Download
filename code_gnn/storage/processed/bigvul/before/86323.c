void __unmap_hugepage_range_final(struct mmu_gather *tlb,
			  struct vm_area_struct *vma, unsigned long start,
			  unsigned long end, struct page *ref_page)
{
	__unmap_hugepage_range(tlb, vma, start, end, ref_page);

	 
	vma->vm_flags &= ~VM_MAYSHARE;
}
