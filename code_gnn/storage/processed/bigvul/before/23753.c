static int bond_set_promiscuity(struct bonding *bond, int inc)
{
	int err = 0;
	if (USES_PRIMARY(bond->params.mode)) {
		 
		if (bond->curr_active_slave) {
			err = dev_set_promiscuity(bond->curr_active_slave->dev,
						  inc);
		}
	} else {
		struct slave *slave;
		int i;
		bond_for_each_slave(bond, slave, i) {
			err = dev_set_promiscuity(slave->dev, inc);
			if (err)
				return err;
		}
	}
	return err;
}