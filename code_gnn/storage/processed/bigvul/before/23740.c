static int bond_release_all(struct net_device *bond_dev)
{
	struct bonding *bond = netdev_priv(bond_dev);
	struct slave *slave;
	struct net_device *slave_dev;
	struct sockaddr addr;

	write_lock_bh(&bond->lock);

	netif_carrier_off(bond_dev);

	if (bond->slave_cnt == 0)
		goto out;

	bond->current_arp_slave = NULL;
	bond->primary_slave = NULL;
	bond_change_active_slave(bond, NULL);

	while ((slave = bond->first_slave) != NULL) {
		 
		if (bond->params.mode == BOND_MODE_8023AD)
			bond_3ad_unbind_slave(slave);

		slave_dev = slave->dev;
		bond_detach_slave(bond, slave);

		 
		write_unlock_bh(&bond->lock);

		 
		netdev_rx_handler_unregister(slave_dev);
		synchronize_net();

		if (bond_is_lb(bond)) {
			 
			bond_alb_deinit_slave(bond, slave);
		}

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

		if (!bond->params.fail_over_mac) {
			 
			memcpy(addr.sa_data, slave->perm_hwaddr, ETH_ALEN);
			addr.sa_family = slave_dev->type;
			dev_set_mac_address(slave_dev, &addr);
		}

		kfree(slave);

		 
		write_lock_bh(&bond->lock);
	}

	 
	memset(bond_dev->dev_addr, 0, bond_dev->addr_len);

	if (bond_vlan_used(bond)) {
		pr_warning("%s: Warning: clearing HW address of %s while it still has VLANs.\n",
			   bond_dev->name, bond_dev->name);
		pr_warning("%s: When re-adding slaves, make sure the bond's HW address matches its VLANs'.\n",
			   bond_dev->name);
	}

	pr_info("%s: released all slaves\n", bond_dev->name);

out:
	write_unlock_bh(&bond->lock);

	bond_compute_features(bond);

	return 0;
}
