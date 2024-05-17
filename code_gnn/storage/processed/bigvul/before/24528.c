void b43_dma_handle_txstatus(struct b43_wldev *dev,
			     const struct b43_txstatus *status)
{
	const struct b43_dma_ops *ops;
	struct b43_dmaring *ring;
	struct b43_dmadesc_generic *desc;
	struct b43_dmadesc_meta *meta;
	int slot, firstused;
	bool frame_succeed;

	ring = parse_cookie(dev, status->cookie, &slot);
	if (unlikely(!ring))
		return;
	B43_WARN_ON(!ring->tx);

	 
	firstused = ring->current_slot - ring->used_slots + 1;
	if (firstused < 0)
		firstused = ring->nr_slots + firstused;
	if (unlikely(slot != firstused)) {
		 
		b43dbg(dev->wl, "Out of order TX status report on DMA ring %d. "
		       "Expected %d, but got %d\n",
		       ring->index, firstused, slot);
		return;
	}

	ops = ring->ops;
	while (1) {
		B43_WARN_ON(slot < 0 || slot >= ring->nr_slots);
		desc = ops->idx2desc(ring, slot, &meta);

		if (b43_dma_ptr_is_poisoned(meta->skb)) {
			b43dbg(dev->wl, "Poisoned TX slot %d (first=%d) "
			       "on ring %d\n",
			       slot, firstused, ring->index);
			break;
		}
		if (meta->skb) {
			struct b43_private_tx_info *priv_info =
				b43_get_priv_tx_info(IEEE80211_SKB_CB(meta->skb));

			unmap_descbuffer(ring, meta->dmaaddr, meta->skb->len, 1);
			kfree(priv_info->bouncebuffer);
			priv_info->bouncebuffer = NULL;
		} else {
			unmap_descbuffer(ring, meta->dmaaddr,
					 b43_txhdr_size(dev), 1);
		}

		if (meta->is_last_fragment) {
			struct ieee80211_tx_info *info;

			if (unlikely(!meta->skb)) {
				 
				b43dbg(dev->wl, "TX status unexpected NULL skb "
				       "at slot %d (first=%d) on ring %d\n",
				       slot, firstused, ring->index);
				break;
			}

			info = IEEE80211_SKB_CB(meta->skb);

			 
			frame_succeed = b43_fill_txstatus_report(dev, info, status);
#ifdef CONFIG_B43_DEBUG
			if (frame_succeed)
				ring->nr_succeed_tx_packets++;
			else
				ring->nr_failed_tx_packets++;
			ring->nr_total_packet_tries += status->frame_count;
#endif  
			ieee80211_tx_status(dev->wl->hw, meta->skb);

			 
			meta->skb = B43_DMA_PTR_POISON;
		} else {
			 
			if (unlikely(meta->skb)) {
				b43dbg(dev->wl, "TX status unexpected non-NULL skb "
				       "at slot %d (first=%d) on ring %d\n",
				       slot, firstused, ring->index);
				break;
			}
		}

		 
		ring->used_slots--;

		if (meta->is_last_fragment) {
			 
			break;
		}
		slot = next_slot(ring, slot);
	}
	if (ring->stopped) {
		B43_WARN_ON(free_slots(ring) < TX_SLOTS_PER_FRAME);
		ieee80211_wake_queue(dev->wl->hw, ring->queue_prio);
		ring->stopped = 0;
		if (b43_debug(dev, B43_DBG_DMAVERBOSE)) {
			b43dbg(dev->wl, "Woke up TX ring %d\n", ring->index);
		}
	}
}
