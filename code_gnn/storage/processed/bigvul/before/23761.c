static int bond_slave_netdev_event(unsigned long event,
				   struct net_device *slave_dev)
{
	struct net_device *bond_dev = slave_dev->master;
	struct bonding *bond = netdev_priv(bond_dev);

	switch (event) {
	case NETDEV_UNREGISTER:
		if (bond_dev) {
			if (bond->setup_by_slave)
				bond_release_and_destroy(bond_dev, slave_dev);
			else
				bond_release(bond_dev, slave_dev);
		}
		break;
	case NETDEV_CHANGE:
		if (bond->params.mode == BOND_MODE_8023AD || bond_is_lb(bond)) {
			struct slave *slave;

			slave = bond_get_slave_by_dev(bond, slave_dev);
			if (slave) {
				u32 old_speed = slave->speed;
				u8  old_duplex = slave->duplex;

				bond_update_speed_duplex(slave);

				if (bond_is_lb(bond))
					break;

				if (old_speed != slave->speed)
					bond_3ad_adapter_speed_changed(slave);
				if (old_duplex != slave->duplex)
					bond_3ad_adapter_duplex_changed(slave);
			}
		}

		break;
	case NETDEV_DOWN:
		 
		break;
	case NETDEV_CHANGEMTU:
		 
		break;
	case NETDEV_CHANGENAME:
		 
		break;
	case NETDEV_FEAT_CHANGE:
		bond_compute_features(bond);
		break;
	default:
		break;
	}

	return NOTIFY_DONE;
}
