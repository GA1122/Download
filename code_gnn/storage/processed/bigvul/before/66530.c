static void intr_callback(struct urb *urb)
{
	pegasus_t *pegasus = urb->context;
	struct net_device *net;
	int res, status = urb->status;

	if (!pegasus)
		return;
	net = pegasus->net;

	switch (status) {
	case 0:
		break;
	case -ECONNRESET:	 
	case -ENOENT:
	case -ESHUTDOWN:
		return;
	default:
		 
		netif_dbg(pegasus, timer, net, "intr status %d\n", status);
	}

	if (urb->actual_length >= 6) {
		u8 *d = urb->transfer_buffer;

		 
		if (d[0] & (TX_UNDERRUN|EXCESSIVE_COL
					|LATE_COL|JABBER_TIMEOUT)) {
			pegasus->stats.tx_errors++;
			if (d[0] & TX_UNDERRUN)
				pegasus->stats.tx_fifo_errors++;
			if (d[0] & (EXCESSIVE_COL | JABBER_TIMEOUT))
				pegasus->stats.tx_aborted_errors++;
			if (d[0] & LATE_COL)
				pegasus->stats.tx_window_errors++;
		}

		 

		 
		pegasus->stats.rx_missed_errors += ((d[3] & 0x7f) << 8) | d[4];
	}

	res = usb_submit_urb(urb, GFP_ATOMIC);
	if (res == -ENODEV)
		netif_device_detach(pegasus->net);
	if (res)
		netif_err(pegasus, timer, net,
			  "can't resubmit interrupt urb, %d\n", res);
}
