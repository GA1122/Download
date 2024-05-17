static int bond_ab_arp_inspect(struct bonding *bond, int delta_in_ticks)
{
	struct slave *slave;
	int i, commit = 0;
	unsigned long trans_start;

	bond_for_each_slave(bond, slave, i) {
		slave->new_link = BOND_LINK_NOCHANGE;

		if (slave->link != BOND_LINK_UP) {
			if (time_in_range(jiffies,
				slave_last_rx(bond, slave) - delta_in_ticks,
				slave_last_rx(bond, slave) + delta_in_ticks)) {

				slave->new_link = BOND_LINK_UP;
				commit++;
			}

			continue;
		}

		 
		if (time_in_range(jiffies,
				  slave->jiffies - delta_in_ticks,
				  slave->jiffies + 2 * delta_in_ticks))
			continue;

		 
		if (!bond_is_active_slave(slave) &&
		    !bond->current_arp_slave &&
		    !time_in_range(jiffies,
			slave_last_rx(bond, slave) - delta_in_ticks,
			slave_last_rx(bond, slave) + 3 * delta_in_ticks)) {

			slave->new_link = BOND_LINK_DOWN;
			commit++;
		}

		 
		trans_start = dev_trans_start(slave->dev);
		if (bond_is_active_slave(slave) &&
		    (!time_in_range(jiffies,
			trans_start - delta_in_ticks,
			trans_start + 2 * delta_in_ticks) ||
		     !time_in_range(jiffies,
			slave_last_rx(bond, slave) - delta_in_ticks,
			slave_last_rx(bond, slave) + 2 * delta_in_ticks))) {

			slave->new_link = BOND_LINK_DOWN;
			commit++;
		}
	}

	return commit;
}
