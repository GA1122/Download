static unsigned long move_vma(struct vm_area_struct *vma,
		unsigned long old_addr, unsigned long old_len,
		unsigned long new_len, unsigned long new_addr)
{
	struct mm_struct *mm = vma->vm_mm;
	struct vm_area_struct *new_vma;
	unsigned long vm_flags = vma->vm_flags;
	unsigned long new_pgoff;
	unsigned long moved_len;
	unsigned long excess = 0;
	unsigned long hiwater_vm;
	int split = 0;
	int err;

	 
	if (mm->map_count >= sysctl_max_map_count - 3)
		return -ENOMEM;

	 
	err = ksm_madvise(vma, old_addr, old_addr + old_len,
						MADV_UNMERGEABLE, &vm_flags);
	if (err)
		return err;

	new_pgoff = vma->vm_pgoff + ((old_addr - vma->vm_start) >> PAGE_SHIFT);
	new_vma = copy_vma(&vma, new_addr, new_len, new_pgoff);
	if (!new_vma)
		return -ENOMEM;

	moved_len = move_page_tables(vma, old_addr, new_vma, new_addr, old_len);
	if (moved_len < old_len) {
		 
		move_page_tables(new_vma, new_addr, vma, old_addr, moved_len);
		vma = new_vma;
		old_len = new_len;
		old_addr = new_addr;
		new_addr = -ENOMEM;
	}

	 
	if (vm_flags & VM_ACCOUNT) {
		vma->vm_flags &= ~VM_ACCOUNT;
		excess = vma->vm_end - vma->vm_start - old_len;
		if (old_addr > vma->vm_start &&
		    old_addr + old_len < vma->vm_end)
			split = 1;
	}

	 
	hiwater_vm = mm->hiwater_vm;
	mm->total_vm += new_len >> PAGE_SHIFT;
	vm_stat_account(mm, vma->vm_flags, vma->vm_file, new_len>>PAGE_SHIFT);

	if (do_munmap(mm, old_addr, old_len) < 0) {
		 
		vm_unacct_memory(excess >> PAGE_SHIFT);
		excess = 0;
	}
	mm->hiwater_vm = hiwater_vm;

	 
	if (excess) {
		vma->vm_flags |= VM_ACCOUNT;
		if (split)
			vma->vm_next->vm_flags |= VM_ACCOUNT;
	}

	if (vm_flags & VM_LOCKED) {
		mm->locked_vm += new_len >> PAGE_SHIFT;
		if (new_len > old_len)
			mlock_vma_pages_range(new_vma, new_addr + old_len,
						       new_addr + new_len);
	}

	return new_addr;
}
