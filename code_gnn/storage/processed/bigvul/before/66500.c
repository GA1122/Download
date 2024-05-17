static void read_bulk_callback(struct urb *urb)
{
	rtl8150_t *dev;
	unsigned pkt_len, res;
	struct sk_buff *skb;
	struct net_device *netdev;
	u16 rx_stat;
	int status = urb->status;
	int result;

	dev = urb->context;
	if (!dev)
		return;
	if (test_bit(RTL8150_UNPLUG, &dev->flags))
		return;
	netdev = dev->netdev;
	if (!netif_device_present(netdev))
		return;

	switch (status) {
	case 0:
		break;
	case -ENOENT:
		return;	 
	case -ETIME:
		if (printk_ratelimit())
			dev_warn(&urb->dev->dev, "may be reset is needed?..\n");
		goto goon;
	default:
		if (printk_ratelimit())
			dev_warn(&urb->dev->dev, "Rx status %d\n", status);
		goto goon;
	}

	if (!dev->rx_skb)
		goto resched;
	 
	if (urb->actual_length < 4)
		goto goon;

	res = urb->actual_length;
	rx_stat = le16_to_cpu(*(__le16 *)(urb->transfer_buffer + res - 4));
	pkt_len = res - 4;

	skb_put(dev->rx_skb, pkt_len);
	dev->rx_skb->protocol = eth_type_trans(dev->rx_skb, netdev);
	netif_rx(dev->rx_skb);
	netdev->stats.rx_packets++;
	netdev->stats.rx_bytes += pkt_len;

	spin_lock(&dev->rx_pool_lock);
	skb = pull_skb(dev);
	spin_unlock(&dev->rx_pool_lock);
	if (!skb)
		goto resched;

	dev->rx_skb = skb;
goon:
	usb_fill_bulk_urb(dev->rx_urb, dev->udev, usb_rcvbulkpipe(dev->udev, 1),
		      dev->rx_skb->data, RTL8150_MTU, read_bulk_callback, dev);
	result = usb_submit_urb(dev->rx_urb, GFP_ATOMIC);
	if (result == -ENODEV)
		netif_device_detach(dev->netdev);
	else if (result) {
		set_bit(RX_URB_FAIL, &dev->flags);
		goto resched;
	} else {
		clear_bit(RX_URB_FAIL, &dev->flags);
	}

	return;
resched:
	tasklet_schedule(&dev->tl);
}