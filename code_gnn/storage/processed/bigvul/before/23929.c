static netdev_tx_t pvc_xmit(struct sk_buff *skb, struct net_device *dev)
{
	pvc_device *pvc = dev->ml_priv;

	if (pvc->state.active) {
		if (dev->type == ARPHRD_ETHER) {
			int pad = ETH_ZLEN - skb->len;
			if (pad > 0) {  
				int len = skb->len;
				if (skb_tailroom(skb) < pad)
					if (pskb_expand_head(skb, 0, pad,
							     GFP_ATOMIC)) {
						dev->stats.tx_dropped++;
						dev_kfree_skb(skb);
						return NETDEV_TX_OK;
					}
				skb_put(skb, pad);
				memset(skb->data + len, 0, pad);
			}
			skb->protocol = cpu_to_be16(ETH_P_802_3);
		}
		if (!fr_hard_header(&skb, pvc->dlci)) {
			dev->stats.tx_bytes += skb->len;
			dev->stats.tx_packets++;
			if (pvc->state.fecn)  
				dev->stats.tx_compressed++;
			skb->dev = pvc->frad;
			dev_queue_xmit(skb);
			return NETDEV_TX_OK;
		}
	}

	dev->stats.tx_dropped++;
	dev_kfree_skb(skb);
	return NETDEV_TX_OK;
}