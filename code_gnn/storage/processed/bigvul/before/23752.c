static void bond_set_multicast_list(struct net_device *bond_dev)
{
	struct bonding *bond = netdev_priv(bond_dev);
	struct netdev_hw_addr *ha;
	bool found;

	 
	if ((bond_dev->flags & IFF_PROMISC) && !(bond->flags & IFF_PROMISC))
		 
		bond_set_promiscuity(bond, 1);


	if (!(bond_dev->flags & IFF_PROMISC) && (bond->flags & IFF_PROMISC))
		bond_set_promiscuity(bond, -1);


	 
	if ((bond_dev->flags & IFF_ALLMULTI) && !(bond->flags & IFF_ALLMULTI))
		 
		bond_set_allmulti(bond, 1);


	if (!(bond_dev->flags & IFF_ALLMULTI) && (bond->flags & IFF_ALLMULTI))
		bond_set_allmulti(bond, -1);


	read_lock(&bond->lock);

	bond->flags = bond_dev->flags;

	 
	netdev_for_each_mc_addr(ha, bond_dev) {
		found = bond_addr_in_mc_list(ha->addr, &bond->mc_list,
					     bond_dev->addr_len);
		if (!found)
			bond_mc_add(bond, ha->addr);
	}

	 
	netdev_hw_addr_list_for_each(ha, &bond->mc_list) {
		found = bond_addr_in_mc_list(ha->addr, &bond_dev->mc,
					     bond_dev->addr_len);
		if (!found)
			bond_mc_del(bond, ha->addr);
	}

	 
	__hw_addr_flush(&bond->mc_list);
	__hw_addr_add_multiple(&bond->mc_list, &bond_dev->mc,
			       bond_dev->addr_len, NETDEV_HW_ADDR_T_MULTICAST);

	read_unlock(&bond->lock);
}