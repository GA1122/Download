static void __exit bonding_exit(void)
{
	unregister_netdevice_notifier(&bond_netdev_notifier);
	unregister_inetaddr_notifier(&bond_inetaddr_notifier);

	bond_destroy_sysfs();
	bond_destroy_debugfs();

	rtnl_link_unregister(&bond_link_ops);
	unregister_pernet_subsys(&bond_net_ops);

#ifdef CONFIG_NET_POLL_CONTROLLER
	 
	WARN_ON(atomic_read(&netpoll_block_tx));
#endif
}