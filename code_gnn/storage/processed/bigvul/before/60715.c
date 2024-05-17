static void tower_disconnect (struct usb_interface *interface)
{
	struct lego_usb_tower *dev;
	int minor;

	dev = usb_get_intfdata (interface);
	mutex_lock(&open_disc_mutex);
	usb_set_intfdata (interface, NULL);

	minor = dev->minor;

	 
	usb_deregister_dev (interface, &tower_class);

	mutex_lock(&dev->lock);
	mutex_unlock(&open_disc_mutex);

	 
	if (!dev->open_count) {
		mutex_unlock(&dev->lock);
		tower_delete (dev);
	} else {
		dev->udev = NULL;
		 
		wake_up_interruptible_all(&dev->read_wait);
		wake_up_interruptible_all(&dev->write_wait);
		mutex_unlock(&dev->lock);
	}

	dev_info(&interface->dev, "LEGO USB Tower #%d now disconnected\n",
		 (minor - LEGO_USB_TOWER_MINOR_BASE));
}
