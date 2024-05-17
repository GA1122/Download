static int fwnet_send_packet(struct fwnet_packet_task *ptask)
{
	struct fwnet_device *dev;
	unsigned tx_len;
	struct rfc2734_header *bufhdr;
	unsigned long flags;
	bool free;

	dev = ptask->dev;
	tx_len = ptask->max_payload;
	switch (fwnet_get_hdr_lf(&ptask->hdr)) {
	case RFC2374_HDR_UNFRAG:
		bufhdr = (struct rfc2734_header *)
				skb_push(ptask->skb, RFC2374_UNFRAG_HDR_SIZE);
		put_unaligned_be32(ptask->hdr.w0, &bufhdr->w0);
		break;

	case RFC2374_HDR_FIRSTFRAG:
	case RFC2374_HDR_INTFRAG:
	case RFC2374_HDR_LASTFRAG:
		bufhdr = (struct rfc2734_header *)
				skb_push(ptask->skb, RFC2374_FRAG_HDR_SIZE);
		put_unaligned_be32(ptask->hdr.w0, &bufhdr->w0);
		put_unaligned_be32(ptask->hdr.w1, &bufhdr->w1);
		break;

	default:
		BUG();
	}
	if (ptask->dest_node == IEEE1394_ALL_NODES) {
		u8 *p;
		int generation;
		int node_id;
		unsigned int sw_version;

		 
		generation = dev->card->generation;
		smp_rmb();
		node_id = dev->card->node_id;

		switch (ptask->skb->protocol) {
		default:
			sw_version = RFC2734_SW_VERSION;
			break;
#if IS_ENABLED(CONFIG_IPV6)
		case htons(ETH_P_IPV6):
			sw_version = RFC3146_SW_VERSION;
#endif
		}

		p = skb_push(ptask->skb, IEEE1394_GASP_HDR_SIZE);
		put_unaligned_be32(node_id << 16 | IANA_SPECIFIER_ID >> 8, p);
		put_unaligned_be32((IANA_SPECIFIER_ID & 0xff) << 24
						| sw_version, &p[4]);

		 
		fw_send_request(dev->card, &ptask->transaction,
				TCODE_STREAM_DATA,
				fw_stream_packet_destination_id(3,
						IEEE1394_BROADCAST_CHANNEL, 0),
				generation, SCODE_100, 0ULL, ptask->skb->data,
				tx_len + 8, fwnet_write_complete, ptask);

		spin_lock_irqsave(&dev->lock, flags);

		 
		free = (ptask->outstanding_pkts == 0 && !ptask->enqueued);
		if (!free)
			ptask->enqueued = true;
		else
			dec_queued_datagrams(dev);

		spin_unlock_irqrestore(&dev->lock, flags);

		goto out;
	}

	fw_send_request(dev->card, &ptask->transaction,
			TCODE_WRITE_BLOCK_REQUEST, ptask->dest_node,
			ptask->generation, ptask->speed, ptask->fifo_addr,
			ptask->skb->data, tx_len, fwnet_write_complete, ptask);

	spin_lock_irqsave(&dev->lock, flags);

	 
	free = (ptask->outstanding_pkts == 0 && !ptask->enqueued);
	if (!free)
		ptask->enqueued = true;
	else
		dec_queued_datagrams(dev);

	spin_unlock_irqrestore(&dev->lock, flags);

	netif_trans_update(dev->netdev);
 out:
	if (free)
		fwnet_free_ptask(ptask);

	return 0;
}