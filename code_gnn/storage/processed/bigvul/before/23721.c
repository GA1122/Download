static void bond_mc_add(struct bonding *bond, void *addr)
{
	if (USES_PRIMARY(bond->params.mode)) {
		 
		if (bond->curr_active_slave)
			dev_mc_add(bond->curr_active_slave->dev, addr);
	} else {
		struct slave *slave;
		int i;

		bond_for_each_slave(bond, slave, i)
			dev_mc_add(slave->dev, addr);
	}
}