static void efx_enqueue_unwind(struct efx_tx_queue *tx_queue)
{
	struct efx_tx_buffer *buffer;
	dma_addr_t unmap_addr;

	 
	while (tx_queue->insert_count != tx_queue->write_count) {
		--tx_queue->insert_count;
		buffer = &tx_queue->buffer[tx_queue->insert_count &
					   tx_queue->ptr_mask];
		efx_tsoh_free(tx_queue, buffer);
		EFX_BUG_ON_PARANOID(buffer->skb);
		if (buffer->unmap_len) {
			unmap_addr = (buffer->dma_addr + buffer->len -
				      buffer->unmap_len);
			if (buffer->unmap_single)
				pci_unmap_single(tx_queue->efx->pci_dev,
						 unmap_addr, buffer->unmap_len,
						 PCI_DMA_TODEVICE);
			else
				pci_unmap_page(tx_queue->efx->pci_dev,
					       unmap_addr, buffer->unmap_len,
					       PCI_DMA_TODEVICE);
			buffer->unmap_len = 0;
		}
		buffer->len = 0;
		buffer->continuation = true;
	}
}
