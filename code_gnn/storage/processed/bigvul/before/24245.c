static int ath6kl_init_netdev_wmi(struct net_device *dev)
{
	if (!eppingtest && bypasswmi)
		return 0;

	return __ath6kl_init_netdev(dev);
}
