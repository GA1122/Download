static void rx_complete (struct urb *urb)
{
	struct sk_buff		*skb = (struct sk_buff *) urb->context;
	struct skb_data		*entry = (struct skb_data *) skb->cb;
	struct usbnet		*dev = entry->dev;
	int			urb_status = urb->status;
	enum skb_state		state;

	skb_put (skb, urb->actual_length);
	state = rx_done;
	entry->urb = NULL;

	switch (urb_status) {
	 
	case 0:
		break;

	 
	case -EPIPE:
		dev->net->stats.rx_errors++;
		usbnet_defer_kevent (dev, EVENT_RX_HALT);

	 
	case -ECONNRESET:		 
	case -ESHUTDOWN:		 
		netif_dbg(dev, ifdown, dev->net,
			  "rx shutdown, code %d\n", urb_status);
		goto block;

	 
	case -EPROTO:
	case -ETIME:
	case -EILSEQ:
		dev->net->stats.rx_errors++;
		if (!timer_pending (&dev->delay)) {
			mod_timer (&dev->delay, jiffies + THROTTLE_JIFFIES);
			netif_dbg(dev, link, dev->net,
				  "rx throttle %d\n", urb_status);
		}
block:
		state = rx_cleanup;
		entry->urb = urb;
		urb = NULL;
		break;

	 
	case -EOVERFLOW:
		dev->net->stats.rx_over_errors++;

	default:
		state = rx_cleanup;
		dev->net->stats.rx_errors++;
		netif_dbg(dev, rx_err, dev->net, "rx status %d\n", urb_status);
		break;
	}

	 
	if (++dev->pkt_cnt > 30) {
		dev->pkt_cnt = 0;
		dev->pkt_err = 0;
	} else {
		if (state == rx_cleanup)
			dev->pkt_err++;
		if (dev->pkt_err > 20)
			set_bit(EVENT_RX_KILL, &dev->flags);
	}

	state = defer_bh(dev, skb, &dev->rxq, state);

	if (urb) {
		if (netif_running (dev->net) &&
		    !test_bit (EVENT_RX_HALT, &dev->flags) &&
		    state != unlink_start) {
			rx_submit (dev, urb, GFP_ATOMIC);
			usb_mark_last_busy(dev->udev);
			return;
		}
		usb_free_urb (urb);
	}
	netif_dbg(dev, rx_err, dev->net, "no read resubmitted\n");
}
