static long do_mincore(unsigned long addr, unsigned long pages, unsigned char *vec)
{
	struct vm_area_struct *vma;
	unsigned long end;

	vma = find_vma(current->mm, addr);
	if (!vma || addr < vma->vm_start)
		return -ENOMEM;

	end = min(vma->vm_end, addr + (pages << PAGE_SHIFT));

	if (is_vm_hugetlb_page(vma)) {
		mincore_hugetlb_page_range(vma, addr, end, vec);
		return (end - addr) >> PAGE_SHIFT;
	}

	end = pmd_addr_end(addr, end);

	if (is_vm_hugetlb_page(vma))
		mincore_hugetlb_page_range(vma, addr, end, vec);
	else
		mincore_page_range(vma, addr, end, vec);

	return (end - addr) >> PAGE_SHIFT;
}
