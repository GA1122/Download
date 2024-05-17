static int bond_update_speed_duplex(struct slave *slave)
{
	struct net_device *slave_dev = slave->dev;
	struct ethtool_cmd etool = { .cmd = ETHTOOL_GSET };
	u32 slave_speed;
	int res;

	 
	slave->speed = SPEED_100;
	slave->duplex = DUPLEX_FULL;

	if (!slave_dev->ethtool_ops || !slave_dev->ethtool_ops->get_settings)
		return -1;

	res = slave_dev->ethtool_ops->get_settings(slave_dev, &etool);
	if (res < 0)
		return -1;

	slave_speed = ethtool_cmd_speed(&etool);
	if (slave_speed == 0 || slave_speed == ((__u32) -1))
		return -1;

	switch (etool.duplex) {
	case DUPLEX_FULL:
	case DUPLEX_HALF:
		break;
	default:
		return -1;
	}

	slave->speed = slave_speed;
	slave->duplex = etool.duplex;

	return 0;
}