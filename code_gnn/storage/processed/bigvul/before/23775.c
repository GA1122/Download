static int bond_xmit_roundrobin(struct sk_buff *skb, struct net_device *bond_dev)
{
	struct bonding *bond = netdev_priv(bond_dev);
	struct slave *slave, *start_at;
	int i, slave_no, res = 1;
	struct iphdr *iph = ip_hdr(skb);

	 
	if ((iph->protocol == IPPROTO_IGMP) &&
	    (skb->protocol == htons(ETH_P_IP))) {

		read_lock(&bond->curr_slave_lock);
		slave = bond->curr_active_slave;
		read_unlock(&bond->curr_slave_lock);

		if (!slave)
			goto out;
	} else {
		 
		slave_no = bond->rr_tx_counter++ % bond->slave_cnt;

		bond_for_each_slave(bond, slave, i) {
			slave_no--;
			if (slave_no < 0)
				break;
		}
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

out:
	if (res) {
		 
		dev_kfree_skb(skb);
	}

	return NETDEV_TX_OK;
}
