static void yurex_disconnect(struct usb_interface *interface)
{
	struct usb_yurex *dev;
	int minor = interface->minor;

	dev = usb_get_intfdata(interface);
	usb_set_intfdata(interface, NULL);

	 
	usb_deregister_dev(interface, &yurex_class);

	 
	mutex_lock(&dev->io_mutex);
	dev->interface = NULL;
	mutex_unlock(&dev->io_mutex);

	 
	kill_fasync(&dev->async_queue, SIGIO, POLL_IN);
	wake_up_interruptible(&dev->waitq);

	 
	kref_put(&dev->kref, yurex_delete);

	dev_info(&interface->dev, "USB YUREX #%d now disconnected\n", minor);
}
