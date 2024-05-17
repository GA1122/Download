static int bond_xmit_xor(struct sk_buff *skb, struct net_device *bond_dev)
{
	struct bonding *bond = netdev_priv(bond_dev);
	struct slave *slave, *start_at;
	int slave_no;
	int i;
	int res = 1;

	slave_no = bond->xmit_hash_policy(skb, bond->slave_cnt);

	bond_for_each_slave(bond, slave, i) {
		slave_no--;
		if (slave_no < 0)
			break;
	}

	start_at = slave;

	bond_for_each_slave_from(bond, slave, i, start_at) {
		if (IS_UP(slave->dev) &&
		    (slave->link == BOND_LINK_UP) &&
		    bond_is_active_slave(slave)) {
			res = bond_dev_queue_xmit(bond, skb, slave->dev);
			break;
		}
	}

	if (res) {
		 
		dev_kfree_skb(skb);
	}

	return NETDEV_TX_OK;
}