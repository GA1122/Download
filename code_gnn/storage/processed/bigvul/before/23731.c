static void bond_netpoll_cleanup(struct net_device *bond_dev)
{
	struct bonding *bond = netdev_priv(bond_dev);

	read_lock(&bond->lock);
	__bond_netpoll_cleanup(bond);
	read_unlock(&bond->lock);
}
