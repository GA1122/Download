static void fm10k_reuse_rx_page(struct fm10k_ring *rx_ring,
				struct fm10k_rx_buffer *old_buff)
{
	struct fm10k_rx_buffer *new_buff;
	u16 nta = rx_ring->next_to_alloc;

	new_buff = &rx_ring->rx_buffer[nta];

	 
	nta++;
	rx_ring->next_to_alloc = (nta < rx_ring->count) ? nta : 0;

	 
	*new_buff = *old_buff;

	 
	dma_sync_single_range_for_device(rx_ring->dev, old_buff->dma,
					 old_buff->page_offset,
					 FM10K_RX_BUFSZ,
					 DMA_FROM_DEVICE);
}
