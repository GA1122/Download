int remap_pfn_range(struct vm_area_struct *vma, unsigned long addr,
		    unsigned long pfn, unsigned long size, pgprot_t prot)
{
	pgd_t *pgd;
	unsigned long next;
	unsigned long end = addr + PAGE_ALIGN(size);
	struct mm_struct *mm = vma->vm_mm;
	int err;

	 
	if (addr == vma->vm_start && end == vma->vm_end) {
		vma->vm_pgoff = pfn;
		vma->vm_flags |= VM_PFN_AT_MMAP;
	} else if (is_cow_mapping(vma->vm_flags))
		return -EINVAL;

	vma->vm_flags |= VM_IO | VM_RESERVED | VM_PFNMAP;

	err = track_pfn_vma_new(vma, &prot, pfn, PAGE_ALIGN(size));
	if (err) {
		 
		vma->vm_flags &= ~(VM_IO | VM_RESERVED | VM_PFNMAP);
		vma->vm_flags &= ~VM_PFN_AT_MMAP;
		return -EINVAL;
	}

	BUG_ON(addr >= end);
	pfn -= addr >> PAGE_SHIFT;
	pgd = pgd_offset(mm, addr);
	flush_cache_range(vma, addr, end);
	do {
		next = pgd_addr_end(addr, end);
		err = remap_pud_range(mm, pgd, addr, next,
				pfn + (addr >> PAGE_SHIFT), prot);
		if (err)
			break;
	} while (pgd++, addr = next, addr != end);

	if (err)
		untrack_pfn_vma(vma, pfn, PAGE_ALIGN(size));

	return err;
}
