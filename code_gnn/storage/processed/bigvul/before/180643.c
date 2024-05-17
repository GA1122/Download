 static void *__dma_alloc_coherent(struct device *dev, size_t size,
 				  dma_addr_t *dma_handle, gfp_t flags,
 				  struct dma_attrs *attrs)
 {
 	if (dev == NULL) {
 		WARN_ONCE(1, "Use an actual device structure for DMA allocation\n");
 		return NULL;
 	}
 
 	if (IS_ENABLED(CONFIG_ZONE_DMA) &&
 	    dev->coherent_dma_mask <= DMA_BIT_MASK(32))
 		flags |= GFP_DMA;
 	if (IS_ENABLED(CONFIG_DMA_CMA) && (flags & __GFP_WAIT)) {
 		struct page *page;
 		void *addr;
 
 		size = PAGE_ALIGN(size);
 		page = dma_alloc_from_contiguous(dev, size >> PAGE_SHIFT,
 							get_order(size));
 		if (!page)
 			return NULL;
  
  		*dma_handle = phys_to_dma(dev, page_to_phys(page));
  		addr = page_address(page);
		if (flags & __GFP_ZERO)
			memset(addr, 0, size);
// 		memset(addr, 0, size);
  		return addr;
  	} else {
  		return swiotlb_alloc_coherent(dev, size, dma_handle, flags);
 	}
 }