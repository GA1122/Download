static void *__dma_alloc(struct device *dev, size_t size,
			 dma_addr_t *dma_handle, gfp_t flags,
			 struct dma_attrs *attrs)
{
	struct page *page;
	void *ptr, *coherent_ptr;
	bool coherent = is_device_dma_coherent(dev);

	size = PAGE_ALIGN(size);

	if (!coherent && !(flags & __GFP_WAIT)) {
		struct page *page = NULL;
		void *addr = __alloc_from_pool(size, &page, flags);

		if (addr)
			*dma_handle = phys_to_dma(dev, page_to_phys(page));

		return addr;
	}

	ptr = __dma_alloc_coherent(dev, size, dma_handle, flags, attrs);
	if (!ptr)
		goto no_mem;

	 
	if (coherent)
		return ptr;

	 
	__dma_flush_range(ptr, ptr + size);

	 
	page = virt_to_page(ptr);
	coherent_ptr = dma_common_contiguous_remap(page, size, VM_USERMAP,
				__get_dma_pgprot(attrs,
					__pgprot(PROT_NORMAL_NC), false),
					NULL);
	if (!coherent_ptr)
		goto no_map;

	return coherent_ptr;

no_map:
	__dma_free_coherent(dev, size, ptr, *dma_handle, attrs);
no_mem:
	*dma_handle = DMA_ERROR_CODE;
	return NULL;
}