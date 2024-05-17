static int bond_xmit_broadcast(struct sk_buff *skb, struct net_device *bond_dev)
{
	struct bonding *bond = netdev_priv(bond_dev);
	struct slave *slave, *start_at;
	struct net_device *tx_dev = NULL;
	int i;
	int res = 1;

	read_lock(&bond->curr_slave_lock);
	start_at = bond->curr_active_slave;
	read_unlock(&bond->curr_slave_lock);

	if (!start_at)
		goto out;

	bond_for_each_slave_from(bond, slave, i, start_at) {
		if (IS_UP(slave->dev) &&
		    (slave->link == BOND_LINK_UP) &&
		    bond_is_active_slave(slave)) {
			if (tx_dev) {
				struct sk_buff *skb2 = skb_clone(skb, GFP_ATOMIC);
				if (!skb2) {
					pr_err("%s: Error: bond_xmit_broadcast(): skb_clone() failed\n",
					       bond_dev->name);
					continue;
				}

				res = bond_dev_queue_xmit(bond, skb2, tx_dev);
				if (res) {
					dev_kfree_skb(skb2);
					continue;
				}
			}
			tx_dev = slave->dev;
		}
	}

	if (tx_dev)
		res = bond_dev_queue_xmit(bond, skb, tx_dev);

out:
	if (res)
		 
		dev_kfree_skb(skb);

	 
	return NETDEV_TX_OK;
}