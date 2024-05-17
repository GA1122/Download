static int efx_tx_queue_insert(struct efx_tx_queue *tx_queue,
			       dma_addr_t dma_addr, unsigned len,
			       struct efx_tx_buffer **final_buffer)
{
	struct efx_tx_buffer *buffer;
	struct efx_nic *efx = tx_queue->efx;
	unsigned dma_len, fill_level, insert_ptr;
	int q_space;

	EFX_BUG_ON_PARANOID(len <= 0);

	fill_level = tx_queue->insert_count - tx_queue->old_read_count;
	 
	q_space = efx->txq_entries - 1 - fill_level;

	while (1) {
		if (unlikely(q_space-- <= 0)) {
			 
			netif_tx_stop_queue(tx_queue->core_txq);
			 
			smp_mb();
			tx_queue->old_read_count =
				ACCESS_ONCE(tx_queue->read_count);
			fill_level = (tx_queue->insert_count
				      - tx_queue->old_read_count);
			q_space = efx->txq_entries - 1 - fill_level;
			if (unlikely(q_space-- <= 0)) {
				*final_buffer = NULL;
				return 1;
			}
			smp_mb();
			netif_tx_start_queue(tx_queue->core_txq);
		}

		insert_ptr = tx_queue->insert_count & tx_queue->ptr_mask;
		buffer = &tx_queue->buffer[insert_ptr];
		++tx_queue->insert_count;

		EFX_BUG_ON_PARANOID(tx_queue->insert_count -
				    tx_queue->read_count >=
				    efx->txq_entries);

		efx_tsoh_free(tx_queue, buffer);
		EFX_BUG_ON_PARANOID(buffer->len);
		EFX_BUG_ON_PARANOID(buffer->unmap_len);
		EFX_BUG_ON_PARANOID(buffer->skb);
		EFX_BUG_ON_PARANOID(!buffer->continuation);
		EFX_BUG_ON_PARANOID(buffer->tsoh);

		buffer->dma_addr = dma_addr;

		dma_len = efx_max_tx_len(efx, dma_addr);

		 
		if (dma_len >= len)
			break;

		buffer->len = dma_len;  
		dma_addr += dma_len;
		len -= dma_len;
	}

	EFX_BUG_ON_PARANOID(!len);
	buffer->len = len;
	*final_buffer = buffer;
	return 0;
}
