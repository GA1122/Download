static void l2tp_eth_dev_recv(struct l2tp_session *session, struct sk_buff *skb, int data_len)
{
	struct l2tp_eth_sess *spriv = l2tp_session_priv(session);
	struct net_device *dev = spriv->dev;

	if (session->debug & L2TP_MSG_DATA) {
		unsigned int length;
		int offset;
		u8 *ptr = skb->data;

		length = min(32u, skb->len);
		if (!pskb_may_pull(skb, length))
			goto error;

		printk(KERN_DEBUG "%s: eth recv: ", session->name);

		offset = 0;
		do {
			printk(" %02X", ptr[offset]);
		} while (++offset < length);

		printk("\n");
	}

	if (!pskb_may_pull(skb, sizeof(ETH_HLEN)))
		goto error;

	secpath_reset(skb);

	 
	skb->ip_summed = CHECKSUM_NONE;

	skb_dst_drop(skb);
	nf_reset(skb);

	if (dev_forward_skb(dev, skb) == NET_RX_SUCCESS) {
		dev->stats.rx_packets++;
		dev->stats.rx_bytes += data_len;
	} else
		dev->stats.rx_errors++;

	return;

error:
	dev->stats.rx_errors++;
	kfree_skb(skb);
}