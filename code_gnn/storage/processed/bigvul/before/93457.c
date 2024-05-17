static void rollback_registered_many(struct list_head *head)
{
	struct net_device *dev, *tmp;
	LIST_HEAD(close_head);

	BUG_ON(dev_boot_phase);
	ASSERT_RTNL();

	list_for_each_entry_safe(dev, tmp, head, unreg_list) {
		 
		if (dev->reg_state == NETREG_UNINITIALIZED) {
			pr_debug("unregister_netdevice: device %s/%p never was registered\n",
				 dev->name, dev);

			WARN_ON(1);
			list_del(&dev->unreg_list);
			continue;
		}
		dev->dismantle = true;
		BUG_ON(dev->reg_state != NETREG_REGISTERED);
	}

	 
	list_for_each_entry(dev, head, unreg_list)
		list_add_tail(&dev->close_list, &close_head);
	dev_close_many(&close_head, true);

	list_for_each_entry(dev, head, unreg_list) {
		 
		unlist_netdevice(dev);

		dev->reg_state = NETREG_UNREGISTERING;
	}
	flush_all_backlogs();

	synchronize_net();

	list_for_each_entry(dev, head, unreg_list) {
		struct sk_buff *skb = NULL;

		 
		dev_shutdown(dev);


		 
		call_netdevice_notifiers(NETDEV_UNREGISTER, dev);

		if (!dev->rtnl_link_ops ||
		    dev->rtnl_link_state == RTNL_LINK_INITIALIZED)
			skb = rtmsg_ifinfo_build_skb(RTM_DELLINK, dev, ~0U, 0,
						     GFP_KERNEL);

		 
		dev_uc_flush(dev);
		dev_mc_flush(dev);

		if (dev->netdev_ops->ndo_uninit)
			dev->netdev_ops->ndo_uninit(dev);

		if (skb)
			rtmsg_ifinfo_send(skb, dev, GFP_KERNEL);

		 
		WARN_ON(netdev_has_any_upper_dev(dev));
		WARN_ON(netdev_has_any_lower_dev(dev));

		 
		netdev_unregister_kobject(dev);
#ifdef CONFIG_XPS
		 
		netif_reset_xps_queues_gt(dev, 0);
#endif
	}

	synchronize_net();

	list_for_each_entry(dev, head, unreg_list)
		dev_put(dev);
}