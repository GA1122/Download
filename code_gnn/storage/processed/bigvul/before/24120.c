static int prism2_change_mtu(struct net_device *dev, int new_mtu)
{
	if (new_mtu < PRISM2_MIN_MTU || new_mtu > PRISM2_MAX_MTU)
		return -EINVAL;

	dev->mtu = new_mtu;
	return 0;
}