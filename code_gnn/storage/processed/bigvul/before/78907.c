static int yurex_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	struct usb_yurex *dev;
	struct usb_host_interface *iface_desc;
	struct usb_endpoint_descriptor *endpoint;
	int retval = -ENOMEM;
	DEFINE_WAIT(wait);
	int res;

	 
	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		goto error;
	kref_init(&dev->kref);
	mutex_init(&dev->io_mutex);
	spin_lock_init(&dev->lock);
	init_waitqueue_head(&dev->waitq);

	dev->udev = usb_get_dev(interface_to_usbdev(interface));
	dev->interface = interface;

	 
	iface_desc = interface->cur_altsetting;
	res = usb_find_int_in_endpoint(iface_desc, &endpoint);
	if (res) {
		dev_err(&interface->dev, "Could not find endpoints\n");
		retval = res;
		goto error;
	}

	dev->int_in_endpointAddr = endpoint->bEndpointAddress;

	 
	dev->cntl_urb = usb_alloc_urb(0, GFP_KERNEL);
	if (!dev->cntl_urb)
		goto error;

	 
	dev->cntl_req = kmalloc(YUREX_BUF_SIZE, GFP_KERNEL);
	if (!dev->cntl_req)
		goto error;

	 
	dev->cntl_buffer = usb_alloc_coherent(dev->udev, YUREX_BUF_SIZE,
					      GFP_KERNEL,
					      &dev->cntl_urb->transfer_dma);
	if (!dev->cntl_buffer) {
		dev_err(&interface->dev, "Could not allocate cntl_buffer\n");
		goto error;
	}

	 
	dev->cntl_req->bRequestType = USB_DIR_OUT | USB_TYPE_CLASS |
				      USB_RECIP_INTERFACE;
	dev->cntl_req->bRequest	= HID_REQ_SET_REPORT;
	dev->cntl_req->wValue	= cpu_to_le16((HID_OUTPUT_REPORT + 1) << 8);
	dev->cntl_req->wIndex	= cpu_to_le16(iface_desc->desc.bInterfaceNumber);
	dev->cntl_req->wLength	= cpu_to_le16(YUREX_BUF_SIZE);

	usb_fill_control_urb(dev->cntl_urb, dev->udev,
			     usb_sndctrlpipe(dev->udev, 0),
			     (void *)dev->cntl_req, dev->cntl_buffer,
			     YUREX_BUF_SIZE, yurex_control_callback, dev);
	dev->cntl_urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;


	 
	dev->urb = usb_alloc_urb(0, GFP_KERNEL);
	if (!dev->urb)
		goto error;

	 
	dev->int_buffer = usb_alloc_coherent(dev->udev, YUREX_BUF_SIZE,
					GFP_KERNEL, &dev->urb->transfer_dma);
	if (!dev->int_buffer) {
		dev_err(&interface->dev, "Could not allocate int_buffer\n");
		goto error;
	}

	 
	usb_fill_int_urb(dev->urb, dev->udev,
			 usb_rcvintpipe(dev->udev, dev->int_in_endpointAddr),
			 dev->int_buffer, YUREX_BUF_SIZE, yurex_interrupt,
			 dev, 1);
	dev->urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
	if (usb_submit_urb(dev->urb, GFP_KERNEL)) {
		retval = -EIO;
		dev_err(&interface->dev, "Could not submitting URB\n");
		goto error;
	}

	 
	usb_set_intfdata(interface, dev);
	dev->bbu = -1;

	 
	retval = usb_register_dev(interface, &yurex_class);
	if (retval) {
		dev_err(&interface->dev,
			"Not able to get a minor for this device.\n");
		usb_set_intfdata(interface, NULL);
		goto error;
	}

	dev_info(&interface->dev,
		 "USB YUREX device now attached to Yurex #%d\n",
		 interface->minor);

	return 0;

error:
	if (dev)
		 
		kref_put(&dev->kref, yurex_delete);
	return retval;
}
