SYSCALL_DEFINE3(madvise, unsigned long, start, size_t, len_in, int, behavior)
{
	unsigned long end, tmp;
	struct vm_area_struct * vma, *prev;
	int unmapped_error = 0;
	int error = -EINVAL;
	int write;
	size_t len;

#ifdef CONFIG_MEMORY_FAILURE
	if (behavior == MADV_HWPOISON || behavior == MADV_SOFT_OFFLINE)
		return madvise_hwpoison(behavior, start, start+len_in);
#endif
	if (!madvise_behavior_valid(behavior))
		return error;

	write = madvise_need_mmap_write(behavior);
	if (write)
		down_write(&current->mm->mmap_sem);
	else
		down_read(&current->mm->mmap_sem);

	if (start & ~PAGE_MASK)
		goto out;
	len = (len_in + ~PAGE_MASK) & PAGE_MASK;

	 
	if (len_in && !len)
		goto out;

	end = start + len;
	if (end < start)
		goto out;

	error = 0;
	if (end == start)
		goto out;

	 
	vma = find_vma_prev(current->mm, start, &prev);
	if (vma && start > vma->vm_start)
		prev = vma;

	for (;;) {
		 
		error = -ENOMEM;
		if (!vma)
			goto out;

		 
		if (start < vma->vm_start) {
			unmapped_error = -ENOMEM;
			start = vma->vm_start;
			if (start >= end)
				goto out;
		}

		 
		tmp = vma->vm_end;
		if (end < tmp)
			tmp = end;

		 
		error = madvise_vma(vma, &prev, start, tmp, behavior);
		if (error)
			goto out;
		start = tmp;
		if (prev && start < prev->vm_end)
			start = prev->vm_end;
		error = unmapped_error;
		if (start >= end)
			goto out;
		if (prev)
			vma = prev->vm_next;
		else	 
			vma = find_vma(current->mm, start);
	}
out:
	if (write)
		up_write(&current->mm->mmap_sem);
	else
		up_read(&current->mm->mmap_sem);

	return error;
}
