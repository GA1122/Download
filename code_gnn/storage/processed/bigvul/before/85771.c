SYSCALL_DEFINE3(madvise, unsigned long, start, size_t, len_in, int, behavior)
{
	unsigned long end, tmp;
	struct vm_area_struct *vma, *prev;
	int unmapped_error = 0;
	int error = -EINVAL;
	int write;
	size_t len;
	struct blk_plug plug;

	if (!madvise_behavior_valid(behavior))
		return error;

	if (start & ~PAGE_MASK)
		return error;
	len = (len_in + ~PAGE_MASK) & PAGE_MASK;

	 
	if (len_in && !len)
		return error;

	end = start + len;
	if (end < start)
		return error;

	error = 0;
	if (end == start)
		return error;

#ifdef CONFIG_MEMORY_FAILURE
	if (behavior == MADV_HWPOISON || behavior == MADV_SOFT_OFFLINE)
		return madvise_inject_error(behavior, start, start + len_in);
#endif

	write = madvise_need_mmap_write(behavior);
	if (write) {
		if (down_write_killable(&current->mm->mmap_sem))
			return -EINTR;
	} else {
		down_read(&current->mm->mmap_sem);
	}

	 
	vma = find_vma_prev(current->mm, start, &prev);
	if (vma && start > vma->vm_start)
		prev = vma;

	blk_start_plug(&plug);
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
	blk_finish_plug(&plug);
	if (write)
		up_write(&current->mm->mmap_sem);
	else
		up_read(&current->mm->mmap_sem);

	return error;
}
