static netdev_tx_t hwsim_mon_xmit(struct sk_buff *skb,
					struct net_device *dev)
{
	 
	dev_kfree_skb(skb);
	return NETDEV_TX_OK;
}