void fm10k_alloc_rx_buffers(struct fm10k_ring *rx_ring, u16 cleaned_count)
{
	union fm10k_rx_desc *rx_desc;
	struct fm10k_rx_buffer *bi;
	u16 i = rx_ring->next_to_use;

	 
	if (!cleaned_count)
		return;

	rx_desc = FM10K_RX_DESC(rx_ring, i);
	bi = &rx_ring->rx_buffer[i];
	i -= rx_ring->count;

	do {
		if (!fm10k_alloc_mapped_page(rx_ring, bi))
			break;

		 
		rx_desc->q.pkt_addr = cpu_to_le64(bi->dma + bi->page_offset);

		rx_desc++;
		bi++;
		i++;
		if (unlikely(!i)) {
			rx_desc = FM10K_RX_DESC(rx_ring, 0);
			bi = rx_ring->rx_buffer;
			i -= rx_ring->count;
		}

		 
		rx_desc->d.staterr = 0;

		cleaned_count--;
	} while (cleaned_count);

	i += rx_ring->count;

	if (rx_ring->next_to_use != i) {
		 
		rx_ring->next_to_use = i;

		 
		rx_ring->next_to_alloc = i;

		 
		wmb();

		 
		writel(i, rx_ring->tail);
	}
}
