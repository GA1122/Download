static long madvise_dontneed_free(struct vm_area_struct *vma,
				  struct vm_area_struct **prev,
				  unsigned long start, unsigned long end,
				  int behavior)
{
	*prev = vma;
	if (!can_madv_dontneed_vma(vma))
		return -EINVAL;

	if (!userfaultfd_remove(vma, start, end)) {
		*prev = NULL;  

		down_read(&current->mm->mmap_sem);
		vma = find_vma(current->mm, start);
		if (!vma)
			return -ENOMEM;
		if (start < vma->vm_start) {
			 
			return -ENOMEM;
		}
		if (!can_madv_dontneed_vma(vma))
			return -EINVAL;
		if (end > vma->vm_end) {
			 
			end = vma->vm_end;
		}
		VM_WARN_ON(start >= end);
	}

	if (behavior == MADV_DONTNEED)
		return madvise_dontneed_single_vma(vma, start, end);
	else if (behavior == MADV_FREE)
		return madvise_free_single_vma(vma, start, end);
	else
		return -EINVAL;
}
