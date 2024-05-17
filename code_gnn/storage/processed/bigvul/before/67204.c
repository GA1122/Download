static int queue_pages_test_walk(unsigned long start, unsigned long end,
				struct mm_walk *walk)
{
	struct vm_area_struct *vma = walk->vma;
	struct queue_pages *qp = walk->private;
	unsigned long endvma = vma->vm_end;
	unsigned long flags = qp->flags;

	if (!vma_migratable(vma))
		return 1;

	if (endvma > end)
		endvma = end;
	if (vma->vm_start > start)
		start = vma->vm_start;

	if (!(flags & MPOL_MF_DISCONTIG_OK)) {
		if (!vma->vm_next && vma->vm_end < end)
			return -EFAULT;
		if (qp->prev && qp->prev->vm_end < vma->vm_start)
			return -EFAULT;
	}

	qp->prev = vma;

	if (flags & MPOL_MF_LAZY) {
		 
		if (!is_vm_hugetlb_page(vma) &&
			(vma->vm_flags & (VM_READ | VM_EXEC | VM_WRITE)) &&
			!(vma->vm_flags & VM_MIXEDMAP))
			change_prot_numa(vma, start, endvma);
		return 1;
	}

	 
	if (flags & (MPOL_MF_MOVE | MPOL_MF_MOVE_ALL))
		return 0;
	return 1;
}