static netdev_tx_t rtl8150_start_xmit(struct sk_buff *skb,
					    struct net_device *netdev)
{
	rtl8150_t *dev = netdev_priv(netdev);
	int count, res;

	netif_stop_queue(netdev);
	count = (skb->len < 60) ? 60 : skb->len;
	count = (count & 0x3f) ? count : count + 1;
	dev->tx_skb = skb;
	usb_fill_bulk_urb(dev->tx_urb, dev->udev, usb_sndbulkpipe(dev->udev, 2),
		      skb->data, count, write_bulk_callback, dev);
	if ((res = usb_submit_urb(dev->tx_urb, GFP_ATOMIC))) {
		 
		if (res == -ENODEV)
			netif_device_detach(dev->netdev);
		else {
			dev_warn(&netdev->dev, "failed tx_urb %d\n", res);
			netdev->stats.tx_errors++;
			netif_start_queue(netdev);
		}
	} else {
		netdev->stats.tx_packets++;
		netdev->stats.tx_bytes += skb->len;
		netif_trans_update(netdev);
	}

	return NETDEV_TX_OK;
}