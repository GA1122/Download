static void __arm_dma_free(struct device *dev, size_t size, void *cpu_addr,
			   dma_addr_t handle, struct dma_attrs *attrs,
			   bool is_coherent)
{
	struct page *page = pfn_to_page(dma_to_pfn(dev, handle));

	if (dma_release_from_coherent(dev, get_order(size), cpu_addr))
		return;

	size = PAGE_ALIGN(size);

	if (is_coherent || nommu()) {
		__dma_free_buffer(page, size);
	} else if (__free_from_pool(cpu_addr, size)) {
		return;
	} else if (!IS_ENABLED(CONFIG_DMA_CMA)) {
		__dma_free_remap(cpu_addr, size);
		__dma_free_buffer(page, size);
	} else {
		 
		WARN_ON(irqs_disabled());
		__free_from_contiguous(dev, page, cpu_addr, size);
	}
}