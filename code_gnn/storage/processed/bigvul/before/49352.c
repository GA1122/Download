static void fwnet_transmit_packet_done(struct fwnet_packet_task *ptask)
{
	struct fwnet_device *dev = ptask->dev;
	struct sk_buff *skb = ptask->skb;
	unsigned long flags;
	bool free;

	spin_lock_irqsave(&dev->lock, flags);

	ptask->outstanding_pkts--;

	 
	free = (ptask->outstanding_pkts == 0 && ptask->enqueued);
	if (free)
		dec_queued_datagrams(dev);

	if (ptask->outstanding_pkts == 0) {
		dev->netdev->stats.tx_packets++;
		dev->netdev->stats.tx_bytes += skb->len;
	}

	spin_unlock_irqrestore(&dev->lock, flags);

	if (ptask->outstanding_pkts > 0) {
		u16 dg_size;
		u16 fg_off;
		u16 datagram_label;
		u16 lf;

		 
		lf = fwnet_get_hdr_lf(&ptask->hdr);
		switch (lf) {
		case RFC2374_HDR_LASTFRAG:
		case RFC2374_HDR_UNFRAG:
		default:
			dev_err(&dev->netdev->dev,
				"outstanding packet %x lf %x, header %x,%x\n",
				ptask->outstanding_pkts, lf, ptask->hdr.w0,
				ptask->hdr.w1);
			BUG();

		case RFC2374_HDR_FIRSTFRAG:
			 
			dg_size = fwnet_get_hdr_dg_size(&ptask->hdr);
			fg_off = ptask->max_payload - RFC2374_FRAG_HDR_SIZE;
			datagram_label = fwnet_get_hdr_dgl(&ptask->hdr);
			break;

		case RFC2374_HDR_INTFRAG:
			dg_size = fwnet_get_hdr_dg_size(&ptask->hdr);
			fg_off = fwnet_get_hdr_fg_off(&ptask->hdr)
				  + ptask->max_payload - RFC2374_FRAG_HDR_SIZE;
			datagram_label = fwnet_get_hdr_dgl(&ptask->hdr);
			break;
		}

		if (ptask->dest_node == IEEE1394_ALL_NODES) {
			skb_pull(skb,
				 ptask->max_payload + IEEE1394_GASP_HDR_SIZE);
		} else {
			skb_pull(skb, ptask->max_payload);
		}
		if (ptask->outstanding_pkts > 1) {
			fwnet_make_sf_hdr(&ptask->hdr, RFC2374_HDR_INTFRAG,
					  dg_size, fg_off, datagram_label);
		} else {
			fwnet_make_sf_hdr(&ptask->hdr, RFC2374_HDR_LASTFRAG,
					  dg_size, fg_off, datagram_label);
			ptask->max_payload = skb->len + RFC2374_FRAG_HDR_SIZE;
		}
		fwnet_send_packet(ptask);
	}

	if (free)
		fwnet_free_ptask(ptask);
}