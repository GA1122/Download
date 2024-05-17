static int bond_close(struct net_device *bond_dev)
{
	struct bonding *bond = netdev_priv(bond_dev);

	write_lock_bh(&bond->lock);

	bond->send_peer_notif = 0;

	 
	bond->kill_timers = 1;

	write_unlock_bh(&bond->lock);

	if (bond->params.miimon) {   
		cancel_delayed_work(&bond->mii_work);
	}

	if (bond->params.arp_interval) {   
		cancel_delayed_work(&bond->arp_work);
	}

	switch (bond->params.mode) {
	case BOND_MODE_8023AD:
		cancel_delayed_work(&bond->ad_work);
		break;
	case BOND_MODE_TLB:
	case BOND_MODE_ALB:
		cancel_delayed_work(&bond->alb_work);
		break;
	default:
		break;
	}

	if (delayed_work_pending(&bond->mcast_work))
		cancel_delayed_work(&bond->mcast_work);

	if (bond_is_lb(bond)) {
		 
		bond_alb_deinitialize(bond);
	}
	bond->recv_probe = NULL;

	return 0;
}