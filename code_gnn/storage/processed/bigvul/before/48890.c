void netdev_run_todo(void)
{
	struct list_head list;

	 
	list_replace_init(&net_todo_list, &list);

	__rtnl_unlock();


	 
	if (!list_empty(&list))
		rcu_barrier();

	while (!list_empty(&list)) {
		struct net_device *dev
			= list_first_entry(&list, struct net_device, todo_list);
		list_del(&dev->todo_list);

		rtnl_lock();
		call_netdevice_notifiers(NETDEV_UNREGISTER_FINAL, dev);
		__rtnl_unlock();

		if (unlikely(dev->reg_state != NETREG_UNREGISTERING)) {
			pr_err("network todo '%s' but state %d\n",
			       dev->name, dev->reg_state);
			dump_stack();
			continue;
		}

		dev->reg_state = NETREG_UNREGISTERED;

		netdev_wait_allrefs(dev);

		 
		BUG_ON(netdev_refcnt_read(dev));
		BUG_ON(!list_empty(&dev->ptype_all));
		BUG_ON(!list_empty(&dev->ptype_specific));
		WARN_ON(rcu_access_pointer(dev->ip_ptr));
		WARN_ON(rcu_access_pointer(dev->ip6_ptr));
		WARN_ON(dev->dn_ptr);

		if (dev->destructor)
			dev->destructor(dev);

		 
		rtnl_lock();
		dev_net(dev)->dev_unreg_count--;
		__rtnl_unlock();
		wake_up(&netdev_unregistering_wq);

		 
		kobject_put(&dev->dev.kobj);
	}
}