static int shift_arg_pages(struct vm_area_struct *vma, unsigned long shift)
{
	struct mm_struct *mm = vma->vm_mm;
	unsigned long old_start = vma->vm_start;
	unsigned long old_end = vma->vm_end;
	unsigned long length = old_end - old_start;
	unsigned long new_start = old_start - shift;
	unsigned long new_end = old_end - shift;
	struct mmu_gather tlb;

	BUG_ON(new_start > new_end);

	 
	if (vma != find_vma(mm, new_start))
		return -EFAULT;

	 
	if (vma_adjust(vma, new_start, old_end, vma->vm_pgoff, NULL))
		return -ENOMEM;

	 
	if (length != move_page_tables(vma, old_start,
				       vma, new_start, length, false))
		return -ENOMEM;

	lru_add_drain();
	tlb_gather_mmu(&tlb, mm, 0);
	if (new_end > old_start) {
		 
		free_pgd_range(&tlb, new_end, old_end, new_end,
			vma->vm_next ? vma->vm_next->vm_start : 0);
	} else {
		 
		free_pgd_range(&tlb, old_start, old_end, new_end,
			vma->vm_next ? vma->vm_next->vm_start : 0);
	}
	tlb_finish_mmu(&tlb, new_end, old_end);

	 
	vma_adjust(vma, new_start, new_end, vma->vm_pgoff, NULL);

	return 0;
}
