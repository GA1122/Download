static netdev_tx_t veth_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct net_device *rcv = NULL;
	struct veth_priv *priv, *rcv_priv;
	struct veth_net_stats *stats, *rcv_stats;
	int length;

	priv = netdev_priv(dev);
	rcv = priv->peer;
	rcv_priv = netdev_priv(rcv);

	stats = this_cpu_ptr(priv->stats);
	rcv_stats = this_cpu_ptr(rcv_priv->stats);

	 
	if (skb->ip_summed == CHECKSUM_NONE &&
	    rcv->features & NETIF_F_RXCSUM)
		skb->ip_summed = CHECKSUM_UNNECESSARY;

	length = skb->len;
	if (dev_forward_skb(rcv, skb) != NET_RX_SUCCESS)
		goto rx_drop;

	u64_stats_update_begin(&stats->syncp);
	stats->tx_bytes += length;
	stats->tx_packets++;
	u64_stats_update_end(&stats->syncp);

	u64_stats_update_begin(&rcv_stats->syncp);
	rcv_stats->rx_bytes += length;
	rcv_stats->rx_packets++;
	u64_stats_update_end(&rcv_stats->syncp);

	return NETDEV_TX_OK;

rx_drop:
	u64_stats_update_begin(&rcv_stats->syncp);
	rcv_stats->rx_dropped++;
	u64_stats_update_end(&rcv_stats->syncp);
	return NETDEV_TX_OK;
}