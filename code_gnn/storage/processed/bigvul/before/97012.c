static bool vma_shareable(struct vm_area_struct *vma, unsigned long addr)
{
	unsigned long base = addr & PUD_MASK;
	unsigned long end = base + PUD_SIZE;

	 
	if (vma->vm_flags & VM_MAYSHARE && range_in_vma(vma, base, end))
		return true;
	return false;
}
