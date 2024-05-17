void bond_loadbalance_arp_mon(struct work_struct *work)
{
	struct bonding *bond = container_of(work, struct bonding,
					    arp_work.work);
	struct slave *slave, *oldcurrent;
	int do_failover = 0;
	int delta_in_ticks;
	int i;

	read_lock(&bond->lock);

	delta_in_ticks = msecs_to_jiffies(bond->params.arp_interval);

	if (bond->kill_timers)
		goto out;

	if (bond->slave_cnt == 0)
		goto re_arm;

	read_lock(&bond->curr_slave_lock);
	oldcurrent = bond->curr_active_slave;
	read_unlock(&bond->curr_slave_lock);

	 
	bond_for_each_slave(bond, slave, i) {
		unsigned long trans_start = dev_trans_start(slave->dev);

		if (slave->link != BOND_LINK_UP) {
			if (time_in_range(jiffies,
				trans_start - delta_in_ticks,
				trans_start + delta_in_ticks) &&
			    time_in_range(jiffies,
				slave->dev->last_rx - delta_in_ticks,
				slave->dev->last_rx + delta_in_ticks)) {

				slave->link  = BOND_LINK_UP;
				bond_set_active_slave(slave);

				 
				if (!oldcurrent) {
					pr_info("%s: link status definitely up for interface %s, ",
						bond->dev->name,
						slave->dev->name);
					do_failover = 1;
				} else {
					pr_info("%s: interface %s is now up\n",
						bond->dev->name,
						slave->dev->name);
				}
			}
		} else {
			 

			 
			if (!time_in_range(jiffies,
				trans_start - delta_in_ticks,
				trans_start + 2 * delta_in_ticks) ||
			    !time_in_range(jiffies,
				slave->dev->last_rx - delta_in_ticks,
				slave->dev->last_rx + 2 * delta_in_ticks)) {

				slave->link  = BOND_LINK_DOWN;
				bond_set_backup_slave(slave);

				if (slave->link_failure_count < UINT_MAX)
					slave->link_failure_count++;

				pr_info("%s: interface %s is now down.\n",
					bond->dev->name,
					slave->dev->name);

				if (slave == oldcurrent)
					do_failover = 1;
			}
		}

		 
		if (IS_UP(slave->dev))
			bond_arp_send_all(bond, slave);
	}

	if (do_failover) {
		block_netpoll_tx();
		write_lock_bh(&bond->curr_slave_lock);

		bond_select_active_slave(bond);

		write_unlock_bh(&bond->curr_slave_lock);
		unblock_netpoll_tx();
	}

re_arm:
	if (bond->params.arp_interval)
		queue_delayed_work(bond->wq, &bond->arp_work, delta_in_ticks);
out:
	read_unlock(&bond->lock);
}
