static void read_bulk_callback(struct urb *urb)
{
	pegasus_t *pegasus = urb->context;
	struct net_device *net;
	int rx_status, count = urb->actual_length;
	int status = urb->status;
	u8 *buf = urb->transfer_buffer;
	__u16 pkt_len;

	if (!pegasus)
		return;

	net = pegasus->net;
	if (!netif_device_present(net) || !netif_running(net))
		return;

	switch (status) {
	case 0:
		break;
	case -ETIME:
		netif_dbg(pegasus, rx_err, net, "reset MAC\n");
		pegasus->flags &= ~PEGASUS_RX_BUSY;
		break;
	case -EPIPE:		 
		 
		netif_warn(pegasus, rx_err, net, "no rx stall recovery\n");
		return;
	case -ENOENT:
	case -ECONNRESET:
	case -ESHUTDOWN:
		netif_dbg(pegasus, ifdown, net, "rx unlink, %d\n", status);
		return;
	default:
		netif_dbg(pegasus, rx_err, net, "RX status %d\n", status);
		goto goon;
	}

	if (count < 4)
		goto goon;

	rx_status = buf[count - 2];
	if (rx_status & 0x1e) {
		netif_dbg(pegasus, rx_err, net,
			  "RX packet error %x\n", rx_status);
		pegasus->stats.rx_errors++;
		if (rx_status & 0x06)	 
			pegasus->stats.rx_length_errors++;
		if (rx_status & 0x08)
			pegasus->stats.rx_crc_errors++;
		if (rx_status & 0x10)	 
			pegasus->stats.rx_frame_errors++;
		goto goon;
	}
	if (pegasus->chip == 0x8513) {
		pkt_len = le32_to_cpu(*(__le32 *)urb->transfer_buffer);
		pkt_len &= 0x0fff;
		pegasus->rx_skb->data += 2;
	} else {
		pkt_len = buf[count - 3] << 8;
		pkt_len += buf[count - 4];
		pkt_len &= 0xfff;
		pkt_len -= 4;
	}

	 
	if (pkt_len > PEGASUS_MTU)
		goto goon;

	 
	skb_put(pegasus->rx_skb, pkt_len);
	pegasus->rx_skb->protocol = eth_type_trans(pegasus->rx_skb, net);
	netif_rx(pegasus->rx_skb);
	pegasus->stats.rx_packets++;
	pegasus->stats.rx_bytes += pkt_len;

	if (pegasus->flags & PEGASUS_UNPLUG)
		return;

	pegasus->rx_skb = __netdev_alloc_skb_ip_align(pegasus->net, PEGASUS_MTU,
						      GFP_ATOMIC);

	if (pegasus->rx_skb == NULL)
		goto tl_sched;
goon:
	usb_fill_bulk_urb(pegasus->rx_urb, pegasus->usb,
			  usb_rcvbulkpipe(pegasus->usb, 1),
			  pegasus->rx_skb->data, PEGASUS_MTU,
			  read_bulk_callback, pegasus);
	rx_status = usb_submit_urb(pegasus->rx_urb, GFP_ATOMIC);
	if (rx_status == -ENODEV)
		netif_device_detach(pegasus->net);
	else if (rx_status) {
		pegasus->flags |= PEGASUS_RX_URB_FAIL;
		goto tl_sched;
	} else {
		pegasus->flags &= ~PEGASUS_RX_URB_FAIL;
	}

	return;

tl_sched:
	tasklet_schedule(&pegasus->rx_tl);
}