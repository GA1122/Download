test_ctrl_queue(struct usbtest_dev *dev, struct usbtest_param_32 *param)
{
	struct usb_device	*udev = testdev_to_usbdev(dev);
	struct urb		**urb;
	struct ctrl_ctx		context;
	int			i;

	if (param->sglen == 0 || param->iterations > UINT_MAX / param->sglen)
		return -EOPNOTSUPP;

	spin_lock_init(&context.lock);
	context.dev = dev;
	init_completion(&context.complete);
	context.count = param->sglen * param->iterations;
	context.pending = 0;
	context.status = -ENOMEM;
	context.param = param;
	context.last = -1;

	 
	urb = kcalloc(param->sglen, sizeof(struct urb *), GFP_KERNEL);
	if (!urb)
		return -ENOMEM;
	for (i = 0; i < param->sglen; i++) {
		int			pipe = usb_rcvctrlpipe(udev, 0);
		unsigned		len;
		struct urb		*u;
		struct usb_ctrlrequest	req;
		struct subcase		*reqp;

		 
		int			expected = 0;

		 
		memset(&req, 0, sizeof(req));
		req.bRequest = USB_REQ_GET_DESCRIPTOR;
		req.bRequestType = USB_DIR_IN|USB_RECIP_DEVICE;

		switch (i % NUM_SUBCASES) {
		case 0:		 
			req.wValue = cpu_to_le16(USB_DT_DEVICE << 8);
			len = sizeof(struct usb_device_descriptor);
			break;
		case 1:		 
			req.wValue = cpu_to_le16((USB_DT_CONFIG << 8) | 0);
			len = sizeof(struct usb_config_descriptor);
			break;
		case 2:		 
			req.bRequest = USB_REQ_GET_INTERFACE;
			req.bRequestType = USB_DIR_IN|USB_RECIP_INTERFACE;
			 
			len = 1;
			expected = EPIPE;
			break;
		case 3:		 
			req.bRequest = USB_REQ_GET_STATUS;
			req.bRequestType = USB_DIR_IN|USB_RECIP_INTERFACE;
			 
			len = 2;
			break;
		case 4:		 
			req.bRequest = USB_REQ_GET_STATUS;
			req.bRequestType = USB_DIR_IN|USB_RECIP_DEVICE;
			len = 2;
			break;
		case 5:		 
			req.wValue = cpu_to_le16 (USB_DT_DEVICE_QUALIFIER << 8);
			len = sizeof(struct usb_qualifier_descriptor);
			if (udev->speed != USB_SPEED_HIGH)
				expected = EPIPE;
			break;
		case 6:		 
			req.wValue = cpu_to_le16((USB_DT_CONFIG << 8) | 0);
			len = sizeof(struct usb_config_descriptor);
			len += sizeof(struct usb_interface_descriptor);
			break;
		case 7:		 
			req.wValue = cpu_to_le16 (USB_DT_INTERFACE << 8);
			 
			len = sizeof(struct usb_interface_descriptor);
			expected = -EPIPE;
			break;
		 
		case 8:		 
			req.bRequest = USB_REQ_CLEAR_FEATURE;
			req.bRequestType = USB_RECIP_ENDPOINT;
			 
			 
			len = 0;
			pipe = usb_sndctrlpipe(udev, 0);
			expected = EPIPE;
			break;
		case 9:		 
			req.bRequest = USB_REQ_GET_STATUS;
			req.bRequestType = USB_DIR_IN|USB_RECIP_ENDPOINT;
			 
			len = 2;
			break;
		case 10:	 
			req.wValue = cpu_to_le16((USB_DT_CONFIG << 8) | 0);
			len = 1024;
			expected = -EREMOTEIO;
			break;
		 
		case 11:	 
			req.wValue = cpu_to_le16(USB_DT_ENDPOINT << 8);
			 
			len = sizeof(struct usb_interface_descriptor);
			expected = EPIPE;
			break;
		 
		case 12:	 
			req.wValue = cpu_to_le16(USB_DT_STRING << 8);
			 
			len = sizeof(struct usb_interface_descriptor);
			 
			expected = EREMOTEIO;	 
			break;
		case 13:	 
			req.wValue = cpu_to_le16((USB_DT_CONFIG << 8) | 0);
			 
			if (udev->speed == USB_SPEED_SUPER)
				len = 1024 - 512;
			else
				len = 1024 - udev->descriptor.bMaxPacketSize0;
			expected = -EREMOTEIO;
			break;
		case 14:	 
			req.wValue = cpu_to_le16((USB_DT_DEVICE << 8) | 0);
			 
			len = udev->descriptor.bMaxPacketSize0;
			if (udev->speed == USB_SPEED_SUPER)
				len = 512;
			switch (len) {
			case 8:
				len = 24;
				break;
			case 16:
				len = 32;
				break;
			}
			expected = -EREMOTEIO;
			break;
		case 15:
			req.wValue = cpu_to_le16(USB_DT_BOS << 8);
			if (udev->bos)
				len = le16_to_cpu(udev->bos->desc->wTotalLength);
			else
				len = sizeof(struct usb_bos_descriptor);
			if (le16_to_cpu(udev->descriptor.bcdUSB) < 0x0201)
				expected = -EPIPE;
			break;
		default:
			ERROR(dev, "bogus number of ctrl queue testcases!\n");
			context.status = -EINVAL;
			goto cleanup;
		}
		req.wLength = cpu_to_le16(len);
		urb[i] = u = simple_alloc_urb(udev, pipe, len, 0);
		if (!u)
			goto cleanup;

		reqp = kmalloc(sizeof(*reqp), GFP_KERNEL);
		if (!reqp)
			goto cleanup;
		reqp->setup = req;
		reqp->number = i % NUM_SUBCASES;
		reqp->expected = expected;
		u->setup_packet = (char *) &reqp->setup;

		u->context = &context;
		u->complete = ctrl_complete;
	}

	 
	context.urb = urb;
	spin_lock_irq(&context.lock);
	for (i = 0; i < param->sglen; i++) {
		context.status = usb_submit_urb(urb[i], GFP_ATOMIC);
		if (context.status != 0) {
			ERROR(dev, "can't submit urb[%d], status %d\n",
					i, context.status);
			context.count = context.pending;
			break;
		}
		context.pending++;
	}
	spin_unlock_irq(&context.lock);

	 

	 
	if (context.pending > 0)
		wait_for_completion(&context.complete);

cleanup:
	for (i = 0; i < param->sglen; i++) {
		if (!urb[i])
			continue;
		urb[i]->dev = udev;
		kfree(urb[i]->setup_packet);
		simple_free_urb(urb[i]);
	}
	kfree(urb);
	return context.status;
}