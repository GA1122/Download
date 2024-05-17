struct usb_device *usb_alloc_dev(struct usb_device *parent,
				 struct usb_bus *bus, unsigned port1)
{
	struct usb_device *dev;
	struct usb_hcd *usb_hcd = bus_to_hcd(bus);
	unsigned root_hub = 0;
	unsigned raw_port = port1;

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return NULL;

	if (!usb_get_hcd(usb_hcd)) {
		kfree(dev);
		return NULL;
	}
	 
	if (usb_hcd->driver->alloc_dev && parent &&
		!usb_hcd->driver->alloc_dev(usb_hcd, dev)) {
		usb_put_hcd(bus_to_hcd(bus));
		kfree(dev);
		return NULL;
	}

	device_initialize(&dev->dev);
	dev->dev.bus = &usb_bus_type;
	dev->dev.type = &usb_device_type;
	dev->dev.groups = usb_device_groups;
	 
	dev->dev.dma_mask = bus->sysdev->dma_mask;
	dev->dev.dma_pfn_offset = bus->sysdev->dma_pfn_offset;
	set_dev_node(&dev->dev, dev_to_node(bus->sysdev));
	dev->state = USB_STATE_ATTACHED;
	dev->lpm_disable_count = 1;
	atomic_set(&dev->urbnum, 0);

	INIT_LIST_HEAD(&dev->ep0.urb_list);
	dev->ep0.desc.bLength = USB_DT_ENDPOINT_SIZE;
	dev->ep0.desc.bDescriptorType = USB_DT_ENDPOINT;
	 
	usb_enable_endpoint(dev, &dev->ep0, false);
	dev->can_submit = 1;

	 
	if (unlikely(!parent)) {
		dev->devpath[0] = '0';
		dev->route = 0;

		dev->dev.parent = bus->controller;
		device_set_of_node_from_dev(&dev->dev, bus->sysdev);
		dev_set_name(&dev->dev, "usb%d", bus->busnum);
		root_hub = 1;
	} else {
		 
		if (parent->devpath[0] == '0') {
			snprintf(dev->devpath, sizeof dev->devpath,
				"%d", port1);
			 
			dev->route = 0;
		} else {
			snprintf(dev->devpath, sizeof dev->devpath,
				"%s.%d", parent->devpath, port1);
			 
			if (port1 < 15)
				dev->route = parent->route +
					(port1 << ((parent->level - 1)*4));
			else
				dev->route = parent->route +
					(15 << ((parent->level - 1)*4));
		}

		dev->dev.parent = &parent->dev;
		dev_set_name(&dev->dev, "%d-%s", bus->busnum, dev->devpath);

		if (!parent->parent) {
			 
			raw_port = usb_hcd_find_raw_port_number(usb_hcd,
				port1);
		}
		dev->dev.of_node = usb_of_get_device_node(parent, raw_port);

		 
	}

	dev->portnum = port1;
	dev->bus = bus;
	dev->parent = parent;
	INIT_LIST_HEAD(&dev->filelist);

#ifdef	CONFIG_PM
	pm_runtime_set_autosuspend_delay(&dev->dev,
			usb_autosuspend_delay * 1000);
	dev->connect_time = jiffies;
	dev->active_duration = -jiffies;
#endif
	if (root_hub)	 
		dev->authorized = 1;
	else {
		dev->authorized = !!HCD_DEV_AUTHORIZED(usb_hcd);
		dev->wusb = usb_bus_is_wusb(bus) ? 1 : 0;
	}
	return dev;
}