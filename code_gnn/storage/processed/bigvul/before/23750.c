static int bond_set_mac_address(struct net_device *bond_dev, void *addr)
{
	struct bonding *bond = netdev_priv(bond_dev);
	struct sockaddr *sa = addr, tmp_sa;
	struct slave *slave, *stop_at;
	int res = 0;
	int i;

	if (bond->params.mode == BOND_MODE_ALB)
		return bond_alb_set_mac_address(bond_dev, addr);


	pr_debug("bond=%p, name=%s\n",
		 bond, bond_dev ? bond_dev->name : "None");

	 
	if (bond->params.fail_over_mac == BOND_FOM_ACTIVE)
		return 0;

	if (!is_valid_ether_addr(sa->sa_data))
		return -EADDRNOTAVAIL;

	 

	bond_for_each_slave(bond, slave, i) {
		const struct net_device_ops *slave_ops = slave->dev->netdev_ops;
		pr_debug("slave %p %s\n", slave, slave->dev->name);

		if (slave_ops->ndo_set_mac_address == NULL) {
			res = -EOPNOTSUPP;
			pr_debug("EOPNOTSUPP %s\n", slave->dev->name);
			goto unwind;
		}

		res = dev_set_mac_address(slave->dev, addr);
		if (res) {
			 
			pr_debug("err %d %s\n", res, slave->dev->name);
			goto unwind;
		}
	}

	 
	memcpy(bond_dev->dev_addr, sa->sa_data, bond_dev->addr_len);
	return 0;

unwind:
	memcpy(tmp_sa.sa_data, bond_dev->dev_addr, bond_dev->addr_len);
	tmp_sa.sa_family = bond_dev->type;

	 
	stop_at = slave;
	bond_for_each_slave_from_to(bond, slave, i, bond->first_slave, stop_at) {
		int tmp_res;

		tmp_res = dev_set_mac_address(slave->dev, &tmp_sa);
		if (tmp_res) {
			pr_debug("unwind err %d dev %s\n",
				 tmp_res, slave->dev->name);
		}
	}

	return res;
}
