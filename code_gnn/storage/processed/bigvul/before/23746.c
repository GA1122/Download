static int bond_set_allmulti(struct bonding *bond, int inc)
{
	int err = 0;
	if (USES_PRIMARY(bond->params.mode)) {
		 
		if (bond->curr_active_slave) {
			err = dev_set_allmulti(bond->curr_active_slave->dev,
					       inc);
		}
	} else {
		struct slave *slave;
		int i;
		bond_for_each_slave(bond, slave, i) {
			err = dev_set_allmulti(slave->dev, inc);
			if (err)
				return err;
		}
	}
	return err;
}