unsigned long do_mremap(unsigned long addr,
	unsigned long old_len, unsigned long new_len,
	unsigned long flags, unsigned long new_addr)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;
	unsigned long ret = -EINVAL;
	unsigned long charged = 0;

	if (flags & ~(MREMAP_FIXED | MREMAP_MAYMOVE))
		goto out;

	if (addr & ~PAGE_MASK)
		goto out;

	old_len = PAGE_ALIGN(old_len);
	new_len = PAGE_ALIGN(new_len);

	 
	if (!new_len)
		goto out;

	if (flags & MREMAP_FIXED) {
		if (flags & MREMAP_MAYMOVE)
			ret = mremap_to(addr, old_len, new_addr, new_len);
		goto out;
	}

	 
	if (old_len >= new_len) {
		ret = do_munmap(mm, addr+new_len, old_len - new_len);
		if (ret && old_len != new_len)
			goto out;
		ret = addr;
		goto out;
	}

	 
	vma = vma_to_resize(addr, old_len, new_len, &charged);
	if (IS_ERR(vma)) {
		ret = PTR_ERR(vma);
		goto out;
	}

	 
	if (old_len == vma->vm_end - addr) {
		 
		if (vma_expandable(vma, new_len - old_len)) {
			int pages = (new_len - old_len) >> PAGE_SHIFT;

			if (vma_adjust(vma, vma->vm_start, addr + new_len,
				       vma->vm_pgoff, NULL)) {
				ret = -ENOMEM;
				goto out;
			}

			mm->total_vm += pages;
			vm_stat_account(mm, vma->vm_flags, vma->vm_file, pages);
			if (vma->vm_flags & VM_LOCKED) {
				mm->locked_vm += pages;
				mlock_vma_pages_range(vma, addr + old_len,
						   addr + new_len);
			}
			ret = addr;
			goto out;
		}
	}

	 
	ret = -ENOMEM;
	if (flags & MREMAP_MAYMOVE) {
		unsigned long map_flags = 0;
		if (vma->vm_flags & VM_MAYSHARE)
			map_flags |= MAP_SHARED;

		new_addr = get_unmapped_area(vma->vm_file, 0, new_len,
					vma->vm_pgoff +
					((addr - vma->vm_start) >> PAGE_SHIFT),
					map_flags);
		if (new_addr & ~PAGE_MASK) {
			ret = new_addr;
			goto out;
		}

		ret = security_file_mmap(NULL, 0, 0, 0, new_addr, 1);
		if (ret)
			goto out;
		ret = move_vma(vma, addr, old_len, new_len, new_addr);
	}
out:
	if (ret & ~PAGE_MASK)
		vm_unacct_memory(charged);
	return ret;
}