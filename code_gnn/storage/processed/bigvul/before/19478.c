netdev_tx_t efx_enqueue_skb(struct efx_tx_queue *tx_queue, struct sk_buff *skb)
{
	struct efx_nic *efx = tx_queue->efx;
	struct pci_dev *pci_dev = efx->pci_dev;
	struct efx_tx_buffer *buffer;
	skb_frag_t *fragment;
	unsigned int len, unmap_len = 0, fill_level, insert_ptr;
	dma_addr_t dma_addr, unmap_addr = 0;
	unsigned int dma_len;
	bool unmap_single;
	int q_space, i = 0;
	netdev_tx_t rc = NETDEV_TX_OK;

	EFX_BUG_ON_PARANOID(tx_queue->write_count != tx_queue->insert_count);

	if (skb_shinfo(skb)->gso_size)
		return efx_enqueue_skb_tso(tx_queue, skb);

	 
	len = skb_headlen(skb);

	 
	if (EFX_WORKAROUND_15592(efx) && skb->len <= 32) {
		EFX_BUG_ON_PARANOID(skb->data_len);
		len = 32 + 1;
		if (skb_pad(skb, len - skb->len))
			return NETDEV_TX_OK;
	}

	fill_level = tx_queue->insert_count - tx_queue->old_read_count;
	q_space = efx->txq_entries - 1 - fill_level;

	 
	unmap_single = true;
	dma_addr = pci_map_single(pci_dev, skb->data, len, PCI_DMA_TODEVICE);

	 
	while (1) {
		if (unlikely(pci_dma_mapping_error(pci_dev, dma_addr)))
			goto pci_err;

		 
		unmap_len = len;
		unmap_addr = dma_addr;

		 
		do {
			if (unlikely(q_space-- <= 0)) {
				 
				netif_tx_stop_queue(tx_queue->core_txq);
				 
				smp_mb();
				tx_queue->old_read_count =
					ACCESS_ONCE(tx_queue->read_count);
				fill_level = (tx_queue->insert_count
					      - tx_queue->old_read_count);
				q_space = efx->txq_entries - 1 - fill_level;
				if (unlikely(q_space-- <= 0)) {
					rc = NETDEV_TX_BUSY;
					goto unwind;
				}
				smp_mb();
				if (likely(!efx->loopback_selftest))
					netif_tx_start_queue(
						tx_queue->core_txq);
			}

			insert_ptr = tx_queue->insert_count & tx_queue->ptr_mask;
			buffer = &tx_queue->buffer[insert_ptr];
			efx_tsoh_free(tx_queue, buffer);
			EFX_BUG_ON_PARANOID(buffer->tsoh);
			EFX_BUG_ON_PARANOID(buffer->skb);
			EFX_BUG_ON_PARANOID(buffer->len);
			EFX_BUG_ON_PARANOID(!buffer->continuation);
			EFX_BUG_ON_PARANOID(buffer->unmap_len);

			dma_len = efx_max_tx_len(efx, dma_addr);
			if (likely(dma_len >= len))
				dma_len = len;

			 
			buffer->len = dma_len;
			buffer->dma_addr = dma_addr;
			len -= dma_len;
			dma_addr += dma_len;
			++tx_queue->insert_count;
		} while (len);

		 
		buffer->unmap_single = unmap_single;
		buffer->unmap_len = unmap_len;
		unmap_len = 0;

		 
		if (i >= skb_shinfo(skb)->nr_frags)
			break;
		fragment = &skb_shinfo(skb)->frags[i];
		len = skb_frag_size(fragment);
		i++;
		 
		unmap_single = false;
		dma_addr = skb_frag_dma_map(&pci_dev->dev, fragment, 0, len,
					    DMA_TO_DEVICE);
	}

	 
	buffer->skb = skb;
	buffer->continuation = false;

	 
	efx_nic_push_buffers(tx_queue);

	return NETDEV_TX_OK;

 pci_err:
	netif_err(efx, tx_err, efx->net_dev,
		  " TX queue %d could not map skb with %d bytes %d "
		  "fragments for DMA\n", tx_queue->queue, skb->len,
		  skb_shinfo(skb)->nr_frags + 1);

	 
	dev_kfree_skb_any(skb);

 unwind:
	 
	while (tx_queue->insert_count != tx_queue->write_count) {
		--tx_queue->insert_count;
		insert_ptr = tx_queue->insert_count & tx_queue->ptr_mask;
		buffer = &tx_queue->buffer[insert_ptr];
		efx_dequeue_buffer(tx_queue, buffer);
		buffer->len = 0;
	}

	 
	if (unmap_len) {
		if (unmap_single)
			pci_unmap_single(pci_dev, unmap_addr, unmap_len,
					 PCI_DMA_TODEVICE);
		else
			pci_unmap_page(pci_dev, unmap_addr, unmap_len,
				       PCI_DMA_TODEVICE);
	}

	return rc;
}
