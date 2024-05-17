static int userfaultfd_register(struct userfaultfd_ctx *ctx,
				unsigned long arg)
{
	struct mm_struct *mm = ctx->mm;
	struct vm_area_struct *vma, *prev, *cur;
	int ret;
	struct uffdio_register uffdio_register;
	struct uffdio_register __user *user_uffdio_register;
	unsigned long vm_flags, new_flags;
	bool found;
	bool basic_ioctls;
	unsigned long start, end, vma_end;

	user_uffdio_register = (struct uffdio_register __user *) arg;

	ret = -EFAULT;
	if (copy_from_user(&uffdio_register, user_uffdio_register,
			   sizeof(uffdio_register)-sizeof(__u64)))
		goto out;

	ret = -EINVAL;
	if (!uffdio_register.mode)
		goto out;
	if (uffdio_register.mode & ~(UFFDIO_REGISTER_MODE_MISSING|
				     UFFDIO_REGISTER_MODE_WP))
		goto out;
	vm_flags = 0;
	if (uffdio_register.mode & UFFDIO_REGISTER_MODE_MISSING)
		vm_flags |= VM_UFFD_MISSING;
	if (uffdio_register.mode & UFFDIO_REGISTER_MODE_WP) {
		vm_flags |= VM_UFFD_WP;
		 
		ret = -EINVAL;
		goto out;
	}

	ret = validate_range(mm, uffdio_register.range.start,
			     uffdio_register.range.len);
	if (ret)
		goto out;

	start = uffdio_register.range.start;
	end = start + uffdio_register.range.len;

	ret = -ENOMEM;
	if (!mmget_not_zero(mm))
		goto out;

	down_write(&mm->mmap_sem);
	vma = find_vma_prev(mm, start, &prev);
	if (!vma)
		goto out_unlock;

	 
	ret = -EINVAL;
	if (vma->vm_start >= end)
		goto out_unlock;

	 
	if (is_vm_hugetlb_page(vma)) {
		unsigned long vma_hpagesize = vma_kernel_pagesize(vma);

		if (start & (vma_hpagesize - 1))
			goto out_unlock;
	}

	 
	found = false;
	basic_ioctls = false;
	for (cur = vma; cur && cur->vm_start < end; cur = cur->vm_next) {
		cond_resched();

		BUG_ON(!!cur->vm_userfaultfd_ctx.ctx ^
		       !!(cur->vm_flags & (VM_UFFD_MISSING | VM_UFFD_WP)));

		 
		ret = -EINVAL;
		if (!vma_can_userfault(cur))
			goto out_unlock;
		 
		if (is_vm_hugetlb_page(cur) && end <= cur->vm_end &&
		    end > cur->vm_start) {
			unsigned long vma_hpagesize = vma_kernel_pagesize(cur);

			ret = -EINVAL;

			if (end & (vma_hpagesize - 1))
				goto out_unlock;
		}

		 
		ret = -EBUSY;
		if (cur->vm_userfaultfd_ctx.ctx &&
		    cur->vm_userfaultfd_ctx.ctx != ctx)
			goto out_unlock;

		 
		if (is_vm_hugetlb_page(cur))
			basic_ioctls = true;

		found = true;
	}
	BUG_ON(!found);

	if (vma->vm_start < start)
		prev = vma;

	ret = 0;
	do {
		cond_resched();

		BUG_ON(!vma_can_userfault(vma));
		BUG_ON(vma->vm_userfaultfd_ctx.ctx &&
		       vma->vm_userfaultfd_ctx.ctx != ctx);

		 
		if (vma->vm_userfaultfd_ctx.ctx == ctx &&
		    (vma->vm_flags & vm_flags) == vm_flags)
			goto skip;

		if (vma->vm_start > start)
			start = vma->vm_start;
		vma_end = min(end, vma->vm_end);

		new_flags = (vma->vm_flags & ~vm_flags) | vm_flags;
		prev = vma_merge(mm, prev, start, vma_end, new_flags,
				 vma->anon_vma, vma->vm_file, vma->vm_pgoff,
				 vma_policy(vma),
				 ((struct vm_userfaultfd_ctx){ ctx }));
		if (prev) {
			vma = prev;
			goto next;
		}
		if (vma->vm_start < start) {
			ret = split_vma(mm, vma, start, 1);
			if (ret)
				break;
		}
		if (vma->vm_end > end) {
			ret = split_vma(mm, vma, end, 0);
			if (ret)
				break;
		}
	next:
		 
		vma->vm_flags = new_flags;
		vma->vm_userfaultfd_ctx.ctx = ctx;

	skip:
		prev = vma;
		start = vma->vm_end;
		vma = vma->vm_next;
	} while (vma && vma->vm_start < end);
out_unlock:
	up_write(&mm->mmap_sem);
	mmput(mm);
	if (!ret) {
		 
		if (put_user(basic_ioctls ? UFFD_API_RANGE_IOCTLS_BASIC :
			     UFFD_API_RANGE_IOCTLS,
			     &user_uffdio_register->ioctls))
			ret = -EFAULT;
	}
out:
	return ret;
}
