int __init addrconf_init(void)
{
	struct inet6_dev *idev;
	int i, err;

	err = ipv6_addr_label_init();
	if (err < 0) {
		pr_crit("%s: cannot initialize default policy table: %d\n",
			__func__, err);
		goto out;
	}

	err = register_pernet_subsys(&addrconf_ops);
	if (err < 0)
		goto out_addrlabel;

	addrconf_wq = create_workqueue("ipv6_addrconf");
	if (!addrconf_wq) {
		err = -ENOMEM;
		goto out_nowq;
	}

	 
	rtnl_lock();
	idev = ipv6_add_dev(init_net.loopback_dev);
	rtnl_unlock();
	if (IS_ERR(idev)) {
		err = PTR_ERR(idev);
		goto errlo;
	}

	for (i = 0; i < IN6_ADDR_HSIZE; i++)
		INIT_HLIST_HEAD(&inet6_addr_lst[i]);

	register_netdevice_notifier(&ipv6_dev_notf);

	addrconf_verify();

	rtnl_af_register(&inet6_ops);

	err = __rtnl_register(PF_INET6, RTM_GETLINK, NULL, inet6_dump_ifinfo,
			      NULL);
	if (err < 0)
		goto errout;

	 
	__rtnl_register(PF_INET6, RTM_NEWADDR, inet6_rtm_newaddr, NULL, NULL);
	__rtnl_register(PF_INET6, RTM_DELADDR, inet6_rtm_deladdr, NULL, NULL);
	__rtnl_register(PF_INET6, RTM_GETADDR, inet6_rtm_getaddr,
			inet6_dump_ifaddr, NULL);
	__rtnl_register(PF_INET6, RTM_GETMULTICAST, NULL,
			inet6_dump_ifmcaddr, NULL);
	__rtnl_register(PF_INET6, RTM_GETANYCAST, NULL,
			inet6_dump_ifacaddr, NULL);
	__rtnl_register(PF_INET6, RTM_GETNETCONF, inet6_netconf_get_devconf,
			inet6_netconf_dump_devconf, NULL);

	ipv6_addr_label_rtnl_register();

	return 0;
errout:
	rtnl_af_unregister(&inet6_ops);
	unregister_netdevice_notifier(&ipv6_dev_notf);
errlo:
	destroy_workqueue(addrconf_wq);
out_nowq:
	unregister_pernet_subsys(&addrconf_ops);
out_addrlabel:
	ipv6_addr_label_cleanup();
out:
	return err;
}
