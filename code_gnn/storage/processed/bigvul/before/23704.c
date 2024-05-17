static void bond_detach_slave(struct bonding *bond, struct slave *slave)
{
	if (slave->next)
		slave->next->prev = slave->prev;

	if (slave->prev)
		slave->prev->next = slave->next;

	if (bond->first_slave == slave) {  
		if (bond->slave_cnt > 1) {  
			bond->first_slave = slave->next;
		} else {
			bond->first_slave = NULL;  
		}
	}

	slave->next = NULL;
	slave->prev = NULL;
	bond->slave_cnt--;
}
