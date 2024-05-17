static struct page *new_vma_page(struct page *page, unsigned long private, int **x)
{
	struct vm_area_struct *vma = (struct vm_area_struct *)private;
	unsigned long uninitialized_var(address);

	while (vma) {
		address = page_address_in_vma(page, vma);
		if (address != -EFAULT)
			break;
		vma = vma->vm_next;
	}

	 
	return alloc_page_vma(GFP_HIGHUSER_MOVABLE, vma, address);
}
