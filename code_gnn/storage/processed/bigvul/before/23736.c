static int bond_open(struct net_device *bond_dev)
{
	struct bonding *bond = netdev_priv(bond_dev);

	bond->kill_timers = 0;

	INIT_DELAYED_WORK(&bond->mcast_work, bond_resend_igmp_join_requests_delayed);

	if (bond_is_lb(bond)) {
		 
		if (bond_alb_initialize(bond, (bond->params.mode == BOND_MODE_ALB))) {
			 
			return -ENOMEM;
		}

		INIT_DELAYED_WORK(&bond->alb_work, bond_alb_monitor);
		queue_delayed_work(bond->wq, &bond->alb_work, 0);
	}

	if (bond->params.miimon) {   
		INIT_DELAYED_WORK(&bond->mii_work, bond_mii_monitor);
		queue_delayed_work(bond->wq, &bond->mii_work, 0);
	}

	if (bond->params.arp_interval) {   
		if (bond->params.mode == BOND_MODE_ACTIVEBACKUP)
			INIT_DELAYED_WORK(&bond->arp_work,
					  bond_activebackup_arp_mon);
		else
			INIT_DELAYED_WORK(&bond->arp_work,
					  bond_loadbalance_arp_mon);

		queue_delayed_work(bond->wq, &bond->arp_work, 0);
		if (bond->params.arp_validate)
			bond->recv_probe = bond_arp_rcv;
	}

	if (bond->params.mode == BOND_MODE_8023AD) {
		INIT_DELAYED_WORK(&bond->ad_work, bond_3ad_state_machine_handler);
		queue_delayed_work(bond->wq, &bond->ad_work, 0);
		 
		bond->recv_probe = bond_3ad_lacpdu_recv;
		bond_3ad_initiate_agg_selection(bond, 1);
	}

	return 0;
}