static u32 bond_fix_features(struct net_device *dev, u32 features)
{
	struct slave *slave;
	struct bonding *bond = netdev_priv(dev);
	u32 mask;
	int i;

	read_lock(&bond->lock);

	if (!bond->first_slave) {
		 
		features |= NETIF_F_VLAN_CHALLENGED;
		goto out;
	}

	mask = features;
	features &= ~NETIF_F_ONE_FOR_ALL;
	features |= NETIF_F_ALL_FOR_ALL;

	bond_for_each_slave(bond, slave, i) {
		features = netdev_increment_features(features,
						     slave->dev->features,
						     mask);
	}

out:
	read_unlock(&bond->lock);
	return features;
}