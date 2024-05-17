static netdev_tx_t bond_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct bonding *bond = netdev_priv(dev);
	netdev_tx_t ret = NETDEV_TX_OK;

	 
	if (is_netpoll_tx_blocked(dev))
		return NETDEV_TX_BUSY;

	read_lock(&bond->lock);

	if (bond->slave_cnt)
		ret = __bond_start_xmit(skb, dev);
	else
		dev_kfree_skb(skb);

	read_unlock(&bond->lock);

	return ret;
}