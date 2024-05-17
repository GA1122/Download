void __init init_dma_coherent_pool_size(unsigned long size)
{
	 
	BUG_ON(atomic_pool.vaddr);

	 
	if (atomic_pool.size == DEFAULT_DMA_COHERENT_POOL_SIZE)
		atomic_pool.size = size;
}
