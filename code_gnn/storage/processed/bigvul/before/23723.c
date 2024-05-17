static void bond_mc_list_flush(struct net_device *bond_dev,
			       struct net_device *slave_dev)
{
	struct bonding *bond = netdev_priv(bond_dev);
	struct netdev_hw_addr *ha;

	netdev_for_each_mc_addr(ha, bond_dev)
		dev_mc_del(slave_dev, ha->addr);

	if (bond->params.mode == BOND_MODE_8023AD) {
		 
		u8 lacpdu_multicast[ETH_ALEN] = MULTICAST_LACPDU_ADDR;

		dev_mc_del(slave_dev, lacpdu_multicast);
	}
}