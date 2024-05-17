netdev_tx_t usbnet_start_xmit (struct sk_buff *skb,
				     struct net_device *net)
{
	struct usbnet		*dev = netdev_priv(net);
	unsigned int			length;
	struct urb		*urb = NULL;
	struct skb_data		*entry;
	struct driver_info	*info = dev->driver_info;
	unsigned long		flags;
	int retval;

	if (skb)
		skb_tx_timestamp(skb);

	if (info->tx_fixup) {
		skb = info->tx_fixup (dev, skb, GFP_ATOMIC);
		if (!skb) {
			 
			if (info->flags & FLAG_MULTI_PACKET)
				goto not_drop;
			netif_dbg(dev, tx_err, dev->net, "can't tx_fixup skb\n");
			goto drop;
		}
	}

	if (!(urb = usb_alloc_urb (0, GFP_ATOMIC))) {
		netif_dbg(dev, tx_err, dev->net, "no urb\n");
		goto drop;
	}

	entry = (struct skb_data *) skb->cb;
	entry->urb = urb;
	entry->dev = dev;

	usb_fill_bulk_urb (urb, dev->udev, dev->out,
			skb->data, skb->len, tx_complete, skb);
	if (dev->can_dma_sg) {
		if (build_dma_sg(skb, urb) < 0)
			goto drop;
	}
	length = urb->transfer_buffer_length;

	 
	if (length % dev->maxpacket == 0) {
		if (!(info->flags & FLAG_SEND_ZLP)) {
			if (!(info->flags & FLAG_MULTI_PACKET)) {
				length++;
				if (skb_tailroom(skb) && !urb->num_sgs) {
					skb->data[skb->len] = 0;
					__skb_put(skb, 1);
				} else if (urb->num_sgs)
					sg_set_buf(&urb->sg[urb->num_sgs++],
							dev->padding_pkt, 1);
			}
		} else
			urb->transfer_flags |= URB_ZERO_PACKET;
	}
	urb->transfer_buffer_length = length;

	if (info->flags & FLAG_MULTI_PACKET) {
		 
		entry->length += length;
		if (WARN_ON_ONCE(entry->length <= 0))
			entry->length = length;
	} else {
		usbnet_set_skb_tx_stats(skb, 1, length);
	}

	spin_lock_irqsave(&dev->txq.lock, flags);
	retval = usb_autopm_get_interface_async(dev->intf);
	if (retval < 0) {
		spin_unlock_irqrestore(&dev->txq.lock, flags);
		goto drop;
	}

#ifdef CONFIG_PM
	 
	if (test_bit(EVENT_DEV_ASLEEP, &dev->flags)) {
		 
		usb_anchor_urb(urb, &dev->deferred);
		 
		netif_stop_queue(net);
		usb_put_urb(urb);
		spin_unlock_irqrestore(&dev->txq.lock, flags);
		netdev_dbg(dev->net, "Delaying transmission for resumption\n");
		goto deferred;
	}
#endif

	switch ((retval = usb_submit_urb (urb, GFP_ATOMIC))) {
	case -EPIPE:
		netif_stop_queue (net);
		usbnet_defer_kevent (dev, EVENT_TX_HALT);
		usb_autopm_put_interface_async(dev->intf);
		break;
	default:
		usb_autopm_put_interface_async(dev->intf);
		netif_dbg(dev, tx_err, dev->net,
			  "tx: submit urb err %d\n", retval);
		break;
	case 0:
		net->trans_start = jiffies;
		__usbnet_queue_skb(&dev->txq, skb, tx_start);
		if (dev->txq.qlen >= TX_QLEN (dev))
			netif_stop_queue (net);
	}
	spin_unlock_irqrestore (&dev->txq.lock, flags);

	if (retval) {
		netif_dbg(dev, tx_err, dev->net, "drop, code %d\n", retval);
drop:
		dev->net->stats.tx_dropped++;
not_drop:
		if (skb)
			dev_kfree_skb_any (skb);
		if (urb) {
			kfree(urb->sg);
			usb_free_urb(urb);
		}
	} else
		netif_dbg(dev, tx_queued, dev->net,
			  "> tx, len %u, type 0x%x\n", length, skb->protocol);
#ifdef CONFIG_PM
deferred:
#endif
	return NETDEV_TX_OK;
}
