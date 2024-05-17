static int prism2_open(struct net_device *dev)
{
	struct hostap_interface *iface;
	local_info_t *local;

	PDEBUG(DEBUG_FLOW, "%s: prism2_open\n", dev->name);

	iface = netdev_priv(dev);
	local = iface->local;

	if (local->no_pri) {
		printk(KERN_DEBUG "%s: could not set interface UP - no PRI "
		       "f/w\n", dev->name);
		return 1;
	}

	if ((local->func->card_present && !local->func->card_present(local)) ||
	    local->hw_downloading)
		return -ENODEV;

	if (!try_module_get(local->hw_module))
		return -ENODEV;
	local->num_dev_open++;

	if (!local->dev_enabled && local->func->hw_enable(dev, 1)) {
		printk(KERN_WARNING "%s: could not enable MAC port\n",
		       dev->name);
		prism2_close(dev);
		return 1;
	}
	if (!local->dev_enabled)
		prism2_callback(local, PRISM2_CALLBACK_ENABLE);
	local->dev_enabled = 1;

	if (dev != local->dev && !(local->dev->flags & IFF_UP)) {
		 
		local->master_dev_auto_open = 1;
		dev_open(local->dev);
	}

	netif_device_attach(dev);
	netif_start_queue(dev);

	return 0;
}