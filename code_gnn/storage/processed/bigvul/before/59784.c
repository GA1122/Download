int usb_reset_configuration(struct usb_device *dev)
{
	int			i, retval;
	struct usb_host_config	*config;
	struct usb_hcd *hcd = bus_to_hcd(dev->bus);

	if (dev->state == USB_STATE_SUSPENDED)
		return -EHOSTUNREACH;

	 

	for (i = 1; i < 16; ++i) {
		usb_disable_endpoint(dev, i, true);
		usb_disable_endpoint(dev, i + USB_DIR_IN, true);
	}

	config = dev->actconfig;
	retval = 0;
	mutex_lock(hcd->bandwidth_mutex);
	 
	if (usb_disable_lpm(dev)) {
		dev_err(&dev->dev, "%s Failed to disable LPM\n.", __func__);
		mutex_unlock(hcd->bandwidth_mutex);
		return -ENOMEM;
	}
	 
	for (i = 0; i < config->desc.bNumInterfaces; i++) {
		struct usb_interface *intf = config->interface[i];
		struct usb_host_interface *alt;

		alt = usb_altnum_to_altsetting(intf, 0);
		if (!alt)
			alt = &intf->altsetting[0];
		if (alt != intf->cur_altsetting)
			retval = usb_hcd_alloc_bandwidth(dev, NULL,
					intf->cur_altsetting, alt);
		if (retval < 0)
			break;
	}
	 
	if (retval < 0) {
reset_old_alts:
		for (i--; i >= 0; i--) {
			struct usb_interface *intf = config->interface[i];
			struct usb_host_interface *alt;

			alt = usb_altnum_to_altsetting(intf, 0);
			if (!alt)
				alt = &intf->altsetting[0];
			if (alt != intf->cur_altsetting)
				usb_hcd_alloc_bandwidth(dev, NULL,
						alt, intf->cur_altsetting);
		}
		usb_enable_lpm(dev);
		mutex_unlock(hcd->bandwidth_mutex);
		return retval;
	}
	retval = usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
			USB_REQ_SET_CONFIGURATION, 0,
			config->desc.bConfigurationValue, 0,
			NULL, 0, USB_CTRL_SET_TIMEOUT);
	if (retval < 0)
		goto reset_old_alts;
	mutex_unlock(hcd->bandwidth_mutex);

	 
	for (i = 0; i < config->desc.bNumInterfaces; i++) {
		struct usb_interface *intf = config->interface[i];
		struct usb_host_interface *alt;

		alt = usb_altnum_to_altsetting(intf, 0);

		 
		if (!alt)
			alt = &intf->altsetting[0];

		if (alt != intf->cur_altsetting) {
			remove_intf_ep_devs(intf);
			usb_remove_sysfs_intf_files(intf);
		}
		intf->cur_altsetting = alt;
		usb_enable_interface(dev, intf, true);
		if (device_is_registered(&intf->dev)) {
			usb_create_sysfs_intf_files(intf);
			create_intf_ep_devs(intf);
		}
	}
	 
	usb_unlocked_enable_lpm(dev);
	return 0;
}
