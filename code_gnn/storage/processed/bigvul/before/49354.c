static netdev_tx_t fwnet_tx(struct sk_buff *skb, struct net_device *net)
{
	struct fwnet_header hdr_buf;
	struct fwnet_device *dev = netdev_priv(net);
	__be16 proto;
	u16 dest_node;
	unsigned max_payload;
	u16 dg_size;
	u16 *datagram_label_ptr;
	struct fwnet_packet_task *ptask;
	struct fwnet_peer *peer;
	unsigned long flags;

	spin_lock_irqsave(&dev->lock, flags);

	 
	if (netif_queue_stopped(dev->netdev)) {
		spin_unlock_irqrestore(&dev->lock, flags);

		return NETDEV_TX_BUSY;
	}

	ptask = kmem_cache_alloc(fwnet_packet_task_cache, GFP_ATOMIC);
	if (ptask == NULL)
		goto fail;

	skb = skb_share_check(skb, GFP_ATOMIC);
	if (!skb)
		goto fail;

	 
	memcpy(&hdr_buf, skb->data, sizeof(hdr_buf));
	proto = hdr_buf.h_proto;

	switch (proto) {
	case htons(ETH_P_ARP):
	case htons(ETH_P_IP):
#if IS_ENABLED(CONFIG_IPV6)
	case htons(ETH_P_IPV6):
#endif
		break;
	default:
		goto fail;
	}

	skb_pull(skb, sizeof(hdr_buf));
	dg_size = skb->len;

	 
	if (fwnet_hwaddr_is_multicast(hdr_buf.h_dest)) {
		max_payload        = dev->broadcast_xmt_max_payload;
		datagram_label_ptr = &dev->broadcast_xmt_datagramlabel;

		ptask->fifo_addr   = FWNET_NO_FIFO_ADDR;
		ptask->generation  = 0;
		ptask->dest_node   = IEEE1394_ALL_NODES;
		ptask->speed       = SCODE_100;
	} else {
		union fwnet_hwaddr *ha = (union fwnet_hwaddr *)hdr_buf.h_dest;
		__be64 guid = get_unaligned(&ha->uc.uniq_id);
		u8 generation;

		peer = fwnet_peer_find_by_guid(dev, be64_to_cpu(guid));
		if (!peer)
			goto fail;

		generation         = peer->generation;
		dest_node          = peer->node_id;
		max_payload        = peer->max_payload;
		datagram_label_ptr = &peer->datagram_label;

		ptask->fifo_addr   = fwnet_hwaddr_fifo(ha);
		ptask->generation  = generation;
		ptask->dest_node   = dest_node;
		ptask->speed       = peer->speed;
	}

	ptask->hdr.w0 = 0;
	ptask->hdr.w1 = 0;
	ptask->skb = skb;
	ptask->dev = dev;

	 
	if (dg_size <= max_payload) {
		fwnet_make_uf_hdr(&ptask->hdr, ntohs(proto));
		ptask->outstanding_pkts = 1;
		max_payload = dg_size + RFC2374_UNFRAG_HDR_SIZE;
	} else {
		u16 datagram_label;

		max_payload -= RFC2374_FRAG_OVERHEAD;
		datagram_label = (*datagram_label_ptr)++;
		fwnet_make_ff_hdr(&ptask->hdr, ntohs(proto), dg_size,
				  datagram_label);
		ptask->outstanding_pkts = DIV_ROUND_UP(dg_size, max_payload);
		max_payload += RFC2374_FRAG_HDR_SIZE;
	}

	if (++dev->queued_datagrams == FWNET_MAX_QUEUED_DATAGRAMS)
		netif_stop_queue(dev->netdev);

	spin_unlock_irqrestore(&dev->lock, flags);

	ptask->max_payload = max_payload;
	ptask->enqueued    = 0;

	fwnet_send_packet(ptask);

	return NETDEV_TX_OK;

 fail:
	spin_unlock_irqrestore(&dev->lock, flags);

	if (ptask)
		kmem_cache_free(fwnet_packet_task_cache, ptask);

	if (skb != NULL)
		dev_kfree_skb(skb);

	net->stats.tx_dropped++;
	net->stats.tx_errors++;

	 
	return NETDEV_TX_OK;
}