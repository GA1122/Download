static void __handle_link_change(struct usbnet *dev)
{
	if (!test_bit(EVENT_DEV_OPEN, &dev->flags))
		return;

	if (!netif_carrier_ok(dev->net)) {
		 
		unlink_urbs(dev, &dev->rxq);

		 
	} else {
		 
		tasklet_schedule(&dev->bh);
	}

	 
	usbnet_update_max_qlen(dev);

	clear_bit(EVENT_LINK_CHANGE, &dev->flags);
}
