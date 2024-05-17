int follow_hugetlb_page(struct mm_struct *mm, struct vm_area_struct *vma,
			struct page **pages, struct vm_area_struct **vmas,
			unsigned long *position, int *length, int i,
			unsigned int flags)
{
	unsigned long pfn_offset;
	unsigned long vaddr = *position;
	int remainder = *length;
	struct hstate *h = hstate_vma(vma);

	spin_lock(&mm->page_table_lock);
	while (vaddr < vma->vm_end && remainder) {
		pte_t *pte;
		int absent;
		struct page *page;

		 
		pte = huge_pte_offset(mm, vaddr & huge_page_mask(h));
		absent = !pte || huge_pte_none(huge_ptep_get(pte));

		 
		if (absent && (flags & FOLL_DUMP) &&
		    !hugetlbfs_pagecache_present(h, vma, vaddr)) {
			remainder = 0;
			break;
		}

		if (absent ||
		    ((flags & FOLL_WRITE) && !pte_write(huge_ptep_get(pte)))) {
			int ret;

			spin_unlock(&mm->page_table_lock);
			ret = hugetlb_fault(mm, vma, vaddr,
				(flags & FOLL_WRITE) ? FAULT_FLAG_WRITE : 0);
			spin_lock(&mm->page_table_lock);
			if (!(ret & VM_FAULT_ERROR))
				continue;

			remainder = 0;
			break;
		}

		pfn_offset = (vaddr & ~huge_page_mask(h)) >> PAGE_SHIFT;
		page = pte_page(huge_ptep_get(pte));
same_page:
		if (pages) {
			pages[i] = mem_map_offset(page, pfn_offset);
			get_page(pages[i]);
		}

		if (vmas)
			vmas[i] = vma;

		vaddr += PAGE_SIZE;
		++pfn_offset;
		--remainder;
		++i;
		if (vaddr < vma->vm_end && remainder &&
				pfn_offset < pages_per_huge_page(h)) {
			 
			goto same_page;
		}
	}
	spin_unlock(&mm->page_table_lock);
	*length = remainder;
	*position = vaddr;

	return i ? i : -EFAULT;
}
