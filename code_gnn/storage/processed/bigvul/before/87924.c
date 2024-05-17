static struct sk_buff *fm10k_fetch_rx_buffer(struct fm10k_ring *rx_ring,
					     union fm10k_rx_desc *rx_desc,
					     struct sk_buff *skb)
{
	unsigned int size = le16_to_cpu(rx_desc->w.length);
	struct fm10k_rx_buffer *rx_buffer;
	struct page *page;

	rx_buffer = &rx_ring->rx_buffer[rx_ring->next_to_clean];
	page = rx_buffer->page;
	prefetchw(page);

	if (likely(!skb)) {
		void *page_addr = page_address(page) +
				  rx_buffer->page_offset;

		 
		prefetch(page_addr);
#if L1_CACHE_BYTES < 128
		prefetch(page_addr + L1_CACHE_BYTES);
#endif

		 
		skb = napi_alloc_skb(&rx_ring->q_vector->napi,
				     FM10K_RX_HDR_LEN);
		if (unlikely(!skb)) {
			rx_ring->rx_stats.alloc_failed++;
			return NULL;
		}

		 
		prefetchw(skb->data);
	}

	 
	dma_sync_single_range_for_cpu(rx_ring->dev,
				      rx_buffer->dma,
				      rx_buffer->page_offset,
				      size,
				      DMA_FROM_DEVICE);

	 
	if (fm10k_add_rx_frag(rx_buffer, size, rx_desc, skb)) {
		 
		fm10k_reuse_rx_page(rx_ring, rx_buffer);
	} else {
		 
		dma_unmap_page(rx_ring->dev, rx_buffer->dma,
			       PAGE_SIZE, DMA_FROM_DEVICE);
	}

	 
	rx_buffer->page = NULL;

	return skb;
}
