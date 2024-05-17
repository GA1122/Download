static int efx_enqueue_skb_tso(struct efx_tx_queue *tx_queue,
			       struct sk_buff *skb)
{
	struct efx_nic *efx = tx_queue->efx;
	int frag_i, rc, rc2 = NETDEV_TX_OK;
	struct tso_state state;

	 
	state.protocol = efx_tso_check_protocol(skb);

	EFX_BUG_ON_PARANOID(tx_queue->write_count != tx_queue->insert_count);

	tso_start(&state, skb);

	 
	if (skb_headlen(skb) == state.header_len) {
		 
		EFX_BUG_ON_PARANOID(skb_shinfo(skb)->nr_frags < 1);
		frag_i = 0;
		rc = tso_get_fragment(&state, efx,
				      skb_shinfo(skb)->frags + frag_i);
		if (rc)
			goto mem_err;
	} else {
		rc = tso_get_head_fragment(&state, efx, skb);
		if (rc)
			goto mem_err;
		frag_i = -1;
	}

	if (tso_start_new_packet(tx_queue, skb, &state) < 0)
		goto mem_err;

	while (1) {
		rc = tso_fill_packet_with_fragment(tx_queue, skb, &state);
		if (unlikely(rc)) {
			rc2 = NETDEV_TX_BUSY;
			goto unwind;
		}

		 
		if (state.in_len == 0) {
			if (++frag_i >= skb_shinfo(skb)->nr_frags)
				 
				break;
			rc = tso_get_fragment(&state, efx,
					      skb_shinfo(skb)->frags + frag_i);
			if (rc)
				goto mem_err;
		}

		 
		if (state.packet_space == 0 &&
		    tso_start_new_packet(tx_queue, skb, &state) < 0)
			goto mem_err;
	}

	 
	efx_nic_push_buffers(tx_queue);

	tx_queue->tso_bursts++;
	return NETDEV_TX_OK;

 mem_err:
	netif_err(efx, tx_err, efx->net_dev,
		  "Out of memory for TSO headers, or PCI mapping error\n");
	dev_kfree_skb_any(skb);

 unwind:
	 
	if (state.unmap_len) {
		if (state.unmap_single)
			pci_unmap_single(efx->pci_dev, state.unmap_addr,
					 state.unmap_len, PCI_DMA_TODEVICE);
		else
			pci_unmap_page(efx->pci_dev, state.unmap_addr,
				       state.unmap_len, PCI_DMA_TODEVICE);
	}

	efx_enqueue_unwind(tx_queue);
	return rc2;
}
