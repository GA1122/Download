__dma_alloc_remap(struct page *page, size_t size, gfp_t gfp, pgprot_t prot,
	const void *caller)
{
	struct vm_struct *area;
	unsigned long addr;

	 
	area = get_vm_area_caller(size, VM_ARM_DMA_CONSISTENT | VM_USERMAP,
				  caller);
	if (!area)
		return NULL;
	addr = (unsigned long)area->addr;
	area->phys_addr = __pfn_to_phys(page_to_pfn(page));

	if (ioremap_page_range(addr, addr + size, area->phys_addr, prot)) {
		vunmap((void *)addr);
		return NULL;
	}
	return (void *)addr;
}