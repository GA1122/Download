int bond_release(struct net_device *bond_dev, struct net_device *slave_dev)
{
	struct bonding *bond = netdev_priv(bond_dev);
	struct slave *slave, *oldcurrent;
	struct sockaddr addr;
	u32 old_features = bond_dev->features;

	 
	if (!(slave_dev->flags & IFF_SLAVE) ||
	    (slave_dev->master != bond_dev)) {
		pr_err("%s: Error: cannot release %s.\n",
		       bond_dev->name, slave_dev->name);
		return -EINVAL;
	}

	block_netpoll_tx();
	netdev_bonding_change(bond_dev, NETDEV_RELEASE);
	write_lock_bh(&bond->lock);

	slave = bond_get_slave_by_dev(bond, slave_dev);
	if (!slave) {
		 
		pr_info("%s: %s not enslaved\n",
			bond_dev->name, slave_dev->name);
		write_unlock_bh(&bond->lock);
		unblock_netpoll_tx();
		return -EINVAL;
	}

	 
	netdev_rx_handler_unregister(slave_dev);
	write_unlock_bh(&bond->lock);
	synchronize_net();
	write_lock_bh(&bond->lock);

	if (!bond->params.fail_over_mac) {
		if (!compare_ether_addr(bond_dev->dev_addr, slave->perm_hwaddr) &&
		    bond->slave_cnt > 1)
			pr_warning("%s: Warning: the permanent HWaddr of %s - %pM - is still in use by %s. Set the HWaddr of %s to a different address to avoid conflicts.\n",
				   bond_dev->name, slave_dev->name,
				   slave->perm_hwaddr,
				   bond_dev->name, slave_dev->name);
	}

	 
	if (bond->params.mode == BOND_MODE_8023AD) {
		 
		bond_3ad_unbind_slave(slave);
	}

	pr_info("%s: releasing %s interface %s\n",
		bond_dev->name,
		bond_is_active_slave(slave) ? "active" : "backup",
		slave_dev->name);

	oldcurrent = bond->curr_active_slave;

	bond->current_arp_slave = NULL;

	 
	bond_detach_slave(bond, slave);

	if (bond->primary_slave == slave)
		bond->primary_slave = NULL;

	if (oldcurrent == slave)
		bond_change_active_slave(bond, NULL);

	if (bond_is_lb(bond)) {
		 
		write_unlock_bh(&bond->lock);
		bond_alb_deinit_slave(bond, slave);
		write_lock_bh(&bond->lock);
	}

	if (oldcurrent == slave) {
		 
		write_unlock_bh(&bond->lock);
		read_lock(&bond->lock);
		write_lock_bh(&bond->curr_slave_lock);

		bond_select_active_slave(bond);

		write_unlock_bh(&bond->curr_slave_lock);
		read_unlock(&bond->lock);
		write_lock_bh(&bond->lock);
	}

	if (bond->slave_cnt == 0) {
		bond_set_carrier(bond);

		 
		memset(bond_dev->dev_addr, 0, bond_dev->addr_len);

		if (bond_vlan_used(bond)) {
			pr_warning("%s: Warning: clearing HW address of %s while it still has VLANs.\n",
				   bond_dev->name, bond_dev->name);
			pr_warning("%s: When re-adding slaves, make sure the bond's HW address matches its VLANs'.\n",
				   bond_dev->name);
		}
	}

	write_unlock_bh(&bond->lock);
	unblock_netpoll_tx();

	bond_compute_features(bond);
	if (!(bond_dev->features & NETIF_F_VLAN_CHALLENGED) &&
	    (old_features & NETIF_F_VLAN_CHALLENGED))
		pr_info("%s: last VLAN challenged slave %s left bond %s. VLAN blocking is removed\n",
			bond_dev->name, slave_dev->name, bond_dev->name);

	 
	bond_destroy_slave_symlinks(bond_dev, slave_dev);

	bond_del_vlans_from_slave(bond, slave_dev);

	 
	if (!USES_PRIMARY(bond->params.mode)) {
		 
		if (bond_dev->flags & IFF_PROMISC)
			dev_set_promiscuity(slave_dev, -1);

		 
		if (bond_dev->flags & IFF_ALLMULTI)
			dev_set_allmulti(slave_dev, -1);

		 
		netif_addr_lock_bh(bond_dev);
		bond_mc_list_flush(bond_dev, slave_dev);
		netif_addr_unlock_bh(bond_dev);
	}

	netdev_set_bond_master(slave_dev, NULL);

	slave_disable_netpoll(slave);

	 
	dev_close(slave_dev);

	if (bond->params.fail_over_mac != BOND_FOM_ACTIVE) {
		 
		memcpy(addr.sa_data, slave->perm_hwaddr, ETH_ALEN);
		addr.sa_family = slave_dev->type;
		dev_set_mac_address(slave_dev, &addr);
	}

	dev_set_mtu(slave_dev, slave->original_mtu);

	slave_dev->priv_flags &= ~IFF_BONDING;

	kfree(slave);

	return 0;   
}
