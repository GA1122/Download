void bond_mii_monitor(struct work_struct *work)
{
	struct bonding *bond = container_of(work, struct bonding,
					    mii_work.work);
	bool should_notify_peers = false;

	read_lock(&bond->lock);
	if (bond->kill_timers)
		goto out;

	if (bond->slave_cnt == 0)
		goto re_arm;

	should_notify_peers = bond_should_notify_peers(bond);

	if (bond_miimon_inspect(bond)) {
		read_unlock(&bond->lock);
		rtnl_lock();
		read_lock(&bond->lock);

		bond_miimon_commit(bond);

		read_unlock(&bond->lock);
		rtnl_unlock();	 
		read_lock(&bond->lock);
	}

re_arm:
	if (bond->params.miimon)
		queue_delayed_work(bond->wq, &bond->mii_work,
				   msecs_to_jiffies(bond->params.miimon));
out:
	read_unlock(&bond->lock);

	if (should_notify_peers) {
		rtnl_lock();
		netdev_bonding_change(bond->dev, NETDEV_NOTIFY_PEERS);
		rtnl_unlock();
	}
}