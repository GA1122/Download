static int alloc_ringmemory(struct b43_dmaring *ring)
{
	gfp_t flags = GFP_KERNEL;

	 
	if (ring->type == B43_DMA_64BIT)
		flags |= GFP_DMA;
	ring->descbase = dma_alloc_coherent(ring->dev->dev->dma_dev,
					    B43_DMA_RINGMEMSIZE,
					    &(ring->dmabase), flags);
	if (!ring->descbase) {
		b43err(ring->dev->wl, "DMA ringmemory allocation failed\n");
		return -ENOMEM;
	}
	memset(ring->descbase, 0, B43_DMA_RINGMEMSIZE);

	return 0;
}
