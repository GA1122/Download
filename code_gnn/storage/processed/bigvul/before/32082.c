struct net_device *alloc_netdev_mq(int sizeof_priv, const char *name,
		void (*setup)(struct net_device *), unsigned int queue_count)
{
	struct netdev_queue *tx;
	struct net_device *dev;
	size_t alloc_size;
	struct net_device *p;

	BUG_ON(strlen(name) >= sizeof(dev->name));

	alloc_size = sizeof(struct net_device);
	if (sizeof_priv) {
		 
		alloc_size = ALIGN(alloc_size, NETDEV_ALIGN);
		alloc_size += sizeof_priv;
	}
	 
	alloc_size += NETDEV_ALIGN - 1;

	p = kzalloc(alloc_size, GFP_KERNEL);
	if (!p) {
		printk(KERN_ERR "alloc_netdev: Unable to allocate device.\n");
		return NULL;
	}

	tx = kcalloc(queue_count, sizeof(struct netdev_queue), GFP_KERNEL);
	if (!tx) {
		printk(KERN_ERR "alloc_netdev: Unable to allocate "
		       "tx qdiscs.\n");
		goto free_p;
	}

	dev = PTR_ALIGN(p, NETDEV_ALIGN);
	dev->padded = (char *)dev - (char *)p;

	if (dev_addr_init(dev))
		goto free_tx;

	dev_unicast_init(dev);

	dev_net_set(dev, &init_net);

	dev->_tx = tx;
	dev->num_tx_queues = queue_count;
	dev->real_num_tx_queues = queue_count;

	dev->gso_max_size = GSO_MAX_SIZE;

	netdev_init_queues(dev);

	INIT_LIST_HEAD(&dev->ethtool_ntuple_list.list);
	dev->ethtool_ntuple_list.count = 0;
	INIT_LIST_HEAD(&dev->napi_list);
	INIT_LIST_HEAD(&dev->unreg_list);
	INIT_LIST_HEAD(&dev->link_watch_list);
	dev->priv_flags = IFF_XMIT_DST_RELEASE;
	setup(dev);
	strcpy(dev->name, name);
	return dev;

free_tx:
	kfree(tx);

free_p:
	kfree(p);
	return NULL;
}