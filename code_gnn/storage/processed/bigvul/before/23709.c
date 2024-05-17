static struct slave *bond_find_best_slave(struct bonding *bond)
{
	struct slave *new_active, *old_active;
	struct slave *bestslave = NULL;
	int mintime = bond->params.updelay;
	int i;

	new_active = bond->curr_active_slave;

	if (!new_active) {  
		if (bond->slave_cnt > 0)    
			new_active = bond->first_slave;
		else
			return NULL;  
	}

	if ((bond->primary_slave) &&
	    bond->primary_slave->link == BOND_LINK_UP &&
	    bond_should_change_active(bond)) {
		new_active = bond->primary_slave;
	}

	 
	old_active = new_active;

	bond_for_each_slave_from(bond, new_active, i, old_active) {
		if (new_active->link == BOND_LINK_UP) {
			return new_active;
		} else if (new_active->link == BOND_LINK_BACK &&
			   IS_UP(new_active->dev)) {
			 
			if (new_active->delay < mintime) {
				mintime = new_active->delay;
				bestslave = new_active;
			}
		}
	}

	return bestslave;
}
