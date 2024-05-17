static void tun_free_netdev(struct net_device *dev)
{
	struct tun_struct *tun = netdev_priv(dev);

	BUG_ON(!(list_empty(&tun->disabled)));
	free_percpu(tun->pcpu_stats);
	tun_flow_uninit(tun);
	security_tun_dev_free_security(tun->security);
}