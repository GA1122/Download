static netdev_tx_t pegasus_start_xmit(struct sk_buff *skb,
					    struct net_device *net)
{
	pegasus_t *pegasus = netdev_priv(net);
	int count = ((skb->len + 2) & 0x3f) ? skb->len + 2 : skb->len + 3;
	int res;
	__u16 l16 = skb->len;

	netif_stop_queue(net);

	((__le16 *) pegasus->tx_buff)[0] = cpu_to_le16(l16);
	skb_copy_from_linear_data(skb, pegasus->tx_buff + 2, skb->len);
	usb_fill_bulk_urb(pegasus->tx_urb, pegasus->usb,
			  usb_sndbulkpipe(pegasus->usb, 2),
			  pegasus->tx_buff, count,
			  write_bulk_callback, pegasus);
	if ((res = usb_submit_urb(pegasus->tx_urb, GFP_ATOMIC))) {
		netif_warn(pegasus, tx_err, net, "fail tx, %d\n", res);
		switch (res) {
		case -EPIPE:		 
			 
			break;
		case -ENODEV:		 
		case -EPERM:
			netif_device_detach(pegasus->net);
			break;
		default:
			pegasus->stats.tx_errors++;
			netif_start_queue(net);
		}
	} else {
		pegasus->stats.tx_packets++;
		pegasus->stats.tx_bytes += skb->len;
	}
	dev_kfree_skb(skb);

	return NETDEV_TX_OK;
}