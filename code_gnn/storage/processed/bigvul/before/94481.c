static void rfcomm_dev_destruct(struct tty_port *port)
{
	struct rfcomm_dev *dev = container_of(port, struct rfcomm_dev, port);
	struct rfcomm_dlc *dlc = dev->dlc;

	BT_DBG("dev %p dlc %p", dev, dlc);

	 
	BUG_ON(!list_empty(&dev->list));

	rfcomm_dlc_lock(dlc);
	 
	if (dlc->owner == dev)
		dlc->owner = NULL;
	rfcomm_dlc_unlock(dlc);

	rfcomm_dlc_put(dlc);

	tty_unregister_device(rfcomm_tty_driver, dev->id);

	kfree(dev);

	 
	module_put(THIS_MODULE);
}
