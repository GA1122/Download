void usb_disable_device(struct usb_device *dev, int skip_ep0)
{
	int i;
	struct usb_hcd *hcd = bus_to_hcd(dev->bus);

	 
	if (dev->actconfig) {
		 
		for (i = 0; i < dev->actconfig->desc.bNumInterfaces; i++)
			dev->actconfig->interface[i]->unregistering = 1;

		for (i = 0; i < dev->actconfig->desc.bNumInterfaces; i++) {
			struct usb_interface	*interface;

			 
			interface = dev->actconfig->interface[i];
			if (!device_is_registered(&interface->dev))
				continue;
			dev_dbg(&dev->dev, "unregistering interface %s\n",
				dev_name(&interface->dev));
			remove_intf_ep_devs(interface);
			device_del(&interface->dev);
		}

		 
		for (i = 0; i < dev->actconfig->desc.bNumInterfaces; i++) {
			put_device(&dev->actconfig->interface[i]->dev);
			dev->actconfig->interface[i] = NULL;
		}

		if (dev->usb2_hw_lpm_enabled == 1)
			usb_set_usb2_hardware_lpm(dev, 0);
		usb_unlocked_disable_lpm(dev);
		usb_disable_ltm(dev);

		dev->actconfig = NULL;
		if (dev->state == USB_STATE_CONFIGURED)
			usb_set_device_state(dev, USB_STATE_ADDRESS);
	}

	dev_dbg(&dev->dev, "%s nuking %s URBs\n", __func__,
		skip_ep0 ? "non-ep0" : "all");
	if (hcd->driver->check_bandwidth) {
		 
		for (i = skip_ep0; i < 16; ++i) {
			usb_disable_endpoint(dev, i, false);
			usb_disable_endpoint(dev, i + USB_DIR_IN, false);
		}
		 
		mutex_lock(hcd->bandwidth_mutex);
		usb_hcd_alloc_bandwidth(dev, NULL, NULL, NULL);
		mutex_unlock(hcd->bandwidth_mutex);
		 
	}
	for (i = skip_ep0; i < 16; ++i) {
		usb_disable_endpoint(dev, i, true);
		usb_disable_endpoint(dev, i + USB_DIR_IN, true);
	}
}
