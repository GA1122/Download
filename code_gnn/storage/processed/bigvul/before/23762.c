static inline int bond_slave_override(struct bonding *bond,
				      struct sk_buff *skb)
{
	int i, res = 1;
	struct slave *slave = NULL;
	struct slave *check_slave;

	if (!skb->queue_mapping)
		return 1;

	 
	bond_for_each_slave(bond, check_slave, i) {
		if (check_slave->queue_id == skb->queue_mapping) {
			slave = check_slave;
			break;
		}
	}

	 
	if (slave && slave->queue_id && IS_UP(slave->dev) &&
	    (slave->link == BOND_LINK_UP)) {
		res = bond_dev_queue_xmit(bond, skb, slave->dev);
	}

	return res;
}