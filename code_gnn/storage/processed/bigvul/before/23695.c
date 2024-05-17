static int bond_change_mtu(struct net_device *bond_dev, int new_mtu)
{
	struct bonding *bond = netdev_priv(bond_dev);
	struct slave *slave, *stop_at;
	int res = 0;
	int i;

	pr_debug("bond=%p, name=%s, new_mtu=%d\n", bond,
		 (bond_dev ? bond_dev->name : "None"), new_mtu);

	 

	bond_for_each_slave(bond, slave, i) {
		pr_debug("s %p s->p %p c_m %p\n",
			 slave,
			 slave->prev,
			 slave->dev->netdev_ops->ndo_change_mtu);

		res = dev_set_mtu(slave->dev, new_mtu);

		if (res) {
			 
			pr_debug("err %d %s\n", res, slave->dev->name);
			goto unwind;
		}
	}

	bond_dev->mtu = new_mtu;

	return 0;

unwind:
	 
	stop_at = slave;
	bond_for_each_slave_from_to(bond, slave, i, bond->first_slave, stop_at) {
		int tmp_res;

		tmp_res = dev_set_mtu(slave->dev, bond_dev->mtu);
		if (tmp_res) {
			pr_debug("unwind err %d dev %s\n",
				 tmp_res, slave->dev->name);
		}
	}

	return res;
}
