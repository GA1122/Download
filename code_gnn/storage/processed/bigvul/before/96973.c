void adjust_range_if_pmd_sharing_possible(struct vm_area_struct *vma,
				unsigned long *start, unsigned long *end)
{
	unsigned long check_addr = *start;

	if (!(vma->vm_flags & VM_MAYSHARE))
		return;

	for (check_addr = *start; check_addr < *end; check_addr += PUD_SIZE) {
		unsigned long a_start = check_addr & PUD_MASK;
		unsigned long a_end = a_start + PUD_SIZE;

		 
		if (range_in_vma(vma, a_start, a_end)) {
			if (a_start < *start)
				*start = a_start;
			if (a_end > *end)
				*end = a_end;
		}
	}
}
