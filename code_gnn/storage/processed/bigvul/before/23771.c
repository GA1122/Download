static int bond_xmit_activebackup(struct sk_buff *skb, struct net_device *bond_dev)
{
	struct bonding *bond = netdev_priv(bond_dev);
	int res = 1;

	read_lock(&bond->curr_slave_lock);

	if (bond->curr_active_slave)
		res = bond_dev_queue_xmit(bond, skb,
			bond->curr_active_slave->dev);

	if (res)
		 
		dev_kfree_skb(skb);

	read_unlock(&bond->curr_slave_lock);

	return NETDEV_TX_OK;
}