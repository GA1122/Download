static long __get_user_pages(struct task_struct *tsk, struct mm_struct *mm,
		unsigned long start, unsigned long nr_pages,
		unsigned int gup_flags, struct page **pages,
		struct vm_area_struct **vmas, int *nonblocking)
{
	long ret = 0, i = 0;
	struct vm_area_struct *vma = NULL;
	struct follow_page_context ctx = { NULL };

	if (!nr_pages)
		return 0;

	VM_BUG_ON(!!pages != !!(gup_flags & FOLL_GET));

	 
	if (!(gup_flags & FOLL_FORCE))
		gup_flags |= FOLL_NUMA;

	do {
		struct page *page;
		unsigned int foll_flags = gup_flags;
		unsigned int page_increm;

		 
		if (!vma || start >= vma->vm_end) {
			vma = find_extend_vma(mm, start);
			if (!vma && in_gate_area(mm, start)) {
				ret = get_gate_page(mm, start & PAGE_MASK,
						gup_flags, &vma,
						pages ? &pages[i] : NULL);
				if (ret)
					goto out;
				ctx.page_mask = 0;
				goto next_page;
			}

			if (!vma || check_vma_flags(vma, gup_flags)) {
				ret = -EFAULT;
				goto out;
			}
			if (is_vm_hugetlb_page(vma)) {
				i = follow_hugetlb_page(mm, vma, pages, vmas,
						&start, &nr_pages, i,
						gup_flags, nonblocking);
				continue;
			}
		}
retry:
		 
		if (fatal_signal_pending(current)) {
			ret = -ERESTARTSYS;
			goto out;
		}
		cond_resched();

		page = follow_page_mask(vma, start, foll_flags, &ctx);
		if (!page) {
			ret = faultin_page(tsk, vma, start, &foll_flags,
					nonblocking);
			switch (ret) {
			case 0:
				goto retry;
			case -EBUSY:
				ret = 0;
				 
			case -EFAULT:
			case -ENOMEM:
			case -EHWPOISON:
				goto out;
			case -ENOENT:
				goto next_page;
			}
			BUG();
		} else if (PTR_ERR(page) == -EEXIST) {
			 
			goto next_page;
		} else if (IS_ERR(page)) {
			ret = PTR_ERR(page);
			goto out;
		}
		if (pages) {
			pages[i] = page;
			flush_anon_page(vma, page, start);
			flush_dcache_page(page);
			ctx.page_mask = 0;
		}
next_page:
		if (vmas) {
			vmas[i] = vma;
			ctx.page_mask = 0;
		}
		page_increm = 1 + (~(start >> PAGE_SHIFT) & ctx.page_mask);
		if (page_increm > nr_pages)
			page_increm = nr_pages;
		i += page_increm;
		start += page_increm * PAGE_SIZE;
		nr_pages -= page_increm;
	} while (nr_pages);
out:
	if (ctx.pgmap)
		put_dev_pagemap(ctx.pgmap);
	return i ? i : ret;
}