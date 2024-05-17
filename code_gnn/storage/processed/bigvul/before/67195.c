static struct page *new_page(struct page *page, unsigned long start, int **x)
{
	struct vm_area_struct *vma;
	unsigned long uninitialized_var(address);

	vma = find_vma(current->mm, start);
	while (vma) {
		address = page_address_in_vma(page, vma);
		if (address != -EFAULT)
			break;
		vma = vma->vm_next;
	}

	if (PageHuge(page)) {
		BUG_ON(!vma);
		return alloc_huge_page_noerr(vma, address, 1);
	}
	 
	return alloc_page_vma(GFP_HIGHUSER_MOVABLE, vma, address);
}
