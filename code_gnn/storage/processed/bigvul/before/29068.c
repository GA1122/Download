static pfn_t hva_to_pfn(unsigned long addr, bool atomic, bool *async,
			bool write_fault, bool *writable)
{
	struct vm_area_struct *vma;
	pfn_t pfn = 0;
	int npages;

	 
	BUG_ON(atomic && async);

	if (hva_to_pfn_fast(addr, atomic, async, write_fault, writable, &pfn))
		return pfn;

	if (atomic)
		return KVM_PFN_ERR_FAULT;

	npages = hva_to_pfn_slow(addr, async, write_fault, writable, &pfn);
	if (npages == 1)
		return pfn;

	down_read(&current->mm->mmap_sem);
	if (npages == -EHWPOISON ||
	      (!async && check_user_page_hwpoison(addr))) {
		pfn = KVM_PFN_ERR_HWPOISON;
		goto exit;
	}

	vma = find_vma_intersection(current->mm, addr, addr + 1);

	if (vma == NULL)
		pfn = KVM_PFN_ERR_FAULT;
	else if ((vma->vm_flags & VM_PFNMAP)) {
		pfn = ((addr - vma->vm_start) >> PAGE_SHIFT) +
			vma->vm_pgoff;
		BUG_ON(!kvm_is_mmio_pfn(pfn));
	} else {
		if (async && vma_is_valid(vma, write_fault))
			*async = true;
		pfn = KVM_PFN_ERR_FAULT;
	}
exit:
	up_read(&current->mm->mmap_sem);
	return pfn;
}