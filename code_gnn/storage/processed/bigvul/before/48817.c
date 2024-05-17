void free_netdev(struct net_device *dev)
{
	struct napi_struct *p, *n;

	might_sleep();
	netif_free_tx_queues(dev);
#ifdef CONFIG_SYSFS
	kvfree(dev->_rx);
#endif

	kfree(rcu_dereference_protected(dev->ingress_queue, 1));

	 
	dev_addr_flush(dev);

	list_for_each_entry_safe(p, n, &dev->napi_list, dev_list)
		netif_napi_del(p);

	free_percpu(dev->pcpu_refcnt);
	dev->pcpu_refcnt = NULL;

	 
	if (dev->reg_state == NETREG_UNINITIALIZED) {
		netdev_freemem(dev);
		return;
	}

	BUG_ON(dev->reg_state != NETREG_UNREGISTERED);
	dev->reg_state = NETREG_RELEASED;

	 
	put_device(&dev->dev);
}