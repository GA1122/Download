hub_port_init(struct usb_hub *hub, struct usb_device *udev, int port1,
		int retry_counter)
{
	struct usb_device	*hdev = hub->hdev;
	struct usb_hcd		*hcd = bus_to_hcd(hdev->bus);
	struct usb_port		*port_dev = hub->ports[port1 - 1];
	int			retries, operations, retval, i;
	unsigned		delay = HUB_SHORT_RESET_TIME;
	enum usb_device_speed	oldspeed = udev->speed;
	const char		*speed;
	int			devnum = udev->devnum;
	const char		*driver_name;

	 
	if (!hdev->parent) {
		delay = HUB_ROOT_RESET_TIME;
		if (port1 == hdev->bus->otg_port)
			hdev->bus->b_hnp_enable = 0;
	}

	 
	 
	if (oldspeed == USB_SPEED_LOW)
		delay = HUB_LONG_RESET_TIME;

	mutex_lock(hcd->address0_mutex);

	 
	 
	retval = hub_port_reset(hub, port1, udev, delay, false);
	if (retval < 0)		 
		goto fail;
	 

	retval = -ENODEV;

	 
	if (oldspeed != USB_SPEED_UNKNOWN && oldspeed != udev->speed &&
	    !(oldspeed == USB_SPEED_SUPER && udev->speed > oldspeed)) {
		dev_dbg(&udev->dev, "device reset changed speed!\n");
		goto fail;
	}
	oldspeed = udev->speed;

	 
	switch (udev->speed) {
	case USB_SPEED_SUPER_PLUS:
	case USB_SPEED_SUPER:
	case USB_SPEED_WIRELESS:	 
		udev->ep0.desc.wMaxPacketSize = cpu_to_le16(512);
		break;
	case USB_SPEED_HIGH:		 
		udev->ep0.desc.wMaxPacketSize = cpu_to_le16(64);
		break;
	case USB_SPEED_FULL:		 
		 
		udev->ep0.desc.wMaxPacketSize = cpu_to_le16(64);
		break;
	case USB_SPEED_LOW:		 
		udev->ep0.desc.wMaxPacketSize = cpu_to_le16(8);
		break;
	default:
		goto fail;
	}

	if (udev->speed == USB_SPEED_WIRELESS)
		speed = "variable speed Wireless";
	else
		speed = usb_speed_string(udev->speed);

	 
	if (udev->bus->controller->driver)
		driver_name = udev->bus->controller->driver->name;
	else
		driver_name = udev->bus->sysdev->driver->name;

	if (udev->speed < USB_SPEED_SUPER)
		dev_info(&udev->dev,
				"%s %s USB device number %d using %s\n",
				(udev->config) ? "reset" : "new", speed,
				devnum, driver_name);

	 
	if (hdev->tt) {
		udev->tt = hdev->tt;
		udev->ttport = hdev->ttport;
	} else if (udev->speed != USB_SPEED_HIGH
			&& hdev->speed == USB_SPEED_HIGH) {
		if (!hub->tt.hub) {
			dev_err(&udev->dev, "parent hub has no TT\n");
			retval = -EINVAL;
			goto fail;
		}
		udev->tt = &hub->tt;
		udev->ttport = port1;
	}

	 
	for (retries = 0; retries < GET_DESCRIPTOR_TRIES; (++retries, msleep(100))) {
		bool did_new_scheme = false;

		if (use_new_scheme(udev, retry_counter, port_dev)) {
			struct usb_device_descriptor *buf;
			int r = 0;

			did_new_scheme = true;
			retval = hub_enable_device(udev);
			if (retval < 0) {
				dev_err(&udev->dev,
					"hub failed to enable device, error %d\n",
					retval);
				goto fail;
			}

#define GET_DESCRIPTOR_BUFSIZE	64
			buf = kmalloc(GET_DESCRIPTOR_BUFSIZE, GFP_NOIO);
			if (!buf) {
				retval = -ENOMEM;
				continue;
			}

			 
			for (operations = 0; operations < 3; ++operations) {
				buf->bMaxPacketSize0 = 0;
				r = usb_control_msg(udev, usb_rcvaddr0pipe(),
					USB_REQ_GET_DESCRIPTOR, USB_DIR_IN,
					USB_DT_DEVICE << 8, 0,
					buf, GET_DESCRIPTOR_BUFSIZE,
					initial_descriptor_timeout);
				switch (buf->bMaxPacketSize0) {
				case 8: case 16: case 32: case 64: case 255:
					if (buf->bDescriptorType ==
							USB_DT_DEVICE) {
						r = 0;
						break;
					}
					 
				default:
					if (r == 0)
						r = -EPROTO;
					break;
				}
				 
				if (r == 0 || (r == -ETIMEDOUT &&
						retries == 0 &&
						udev->speed > USB_SPEED_FULL))
					break;
			}
			udev->descriptor.bMaxPacketSize0 =
					buf->bMaxPacketSize0;
			kfree(buf);

			retval = hub_port_reset(hub, port1, udev, delay, false);
			if (retval < 0)		 
				goto fail;
			if (oldspeed != udev->speed) {
				dev_dbg(&udev->dev,
					"device reset changed speed!\n");
				retval = -ENODEV;
				goto fail;
			}
			if (r) {
				if (r != -ENODEV)
					dev_err(&udev->dev, "device descriptor read/64, error %d\n",
							r);
				retval = -EMSGSIZE;
				continue;
			}
#undef GET_DESCRIPTOR_BUFSIZE
		}

		 
		if (udev->wusb == 0) {
			for (operations = 0; operations < SET_ADDRESS_TRIES; ++operations) {
				retval = hub_set_address(udev, devnum);
				if (retval >= 0)
					break;
				msleep(200);
			}
			if (retval < 0) {
				if (retval != -ENODEV)
					dev_err(&udev->dev, "device not accepting address %d, error %d\n",
							devnum, retval);
				goto fail;
			}
			if (udev->speed >= USB_SPEED_SUPER) {
				devnum = udev->devnum;
				dev_info(&udev->dev,
						"%s SuperSpeed%s%s USB device number %d using %s\n",
						(udev->config) ? "reset" : "new",
					 (udev->speed == USB_SPEED_SUPER_PLUS) ?
							"Plus Gen 2" : " Gen 1",
					 (udev->rx_lanes == 2 && udev->tx_lanes == 2) ?
							"x2" : "",
					 devnum, driver_name);
			}

			 
			msleep(10);
			 
			if (did_new_scheme)
				break;
		}

		retval = usb_get_device_descriptor(udev, 8);
		if (retval < 8) {
			if (retval != -ENODEV)
				dev_err(&udev->dev,
					"device descriptor read/8, error %d\n",
					retval);
			if (retval >= 0)
				retval = -EMSGSIZE;
		} else {
			u32 delay;

			retval = 0;

			delay = udev->parent->hub_delay;
			udev->hub_delay = min_t(u32, delay,
						USB_TP_TRANSMISSION_DELAY_MAX);
			retval = usb_set_isoch_delay(udev);
			if (retval) {
				dev_dbg(&udev->dev,
					"Failed set isoch delay, error %d\n",
					retval);
				retval = 0;
			}
			break;
		}
	}
	if (retval)
		goto fail;

	 
	if ((udev->speed >= USB_SPEED_SUPER) &&
			(le16_to_cpu(udev->descriptor.bcdUSB) < 0x0300)) {
		dev_err(&udev->dev, "got a wrong device descriptor, "
				"warm reset device\n");
		hub_port_reset(hub, port1, udev,
				HUB_BH_RESET_TIME, true);
		retval = -EINVAL;
		goto fail;
	}

	if (udev->descriptor.bMaxPacketSize0 == 0xff ||
			udev->speed >= USB_SPEED_SUPER)
		i = 512;
	else
		i = udev->descriptor.bMaxPacketSize0;
	if (usb_endpoint_maxp(&udev->ep0.desc) != i) {
		if (udev->speed == USB_SPEED_LOW ||
				!(i == 8 || i == 16 || i == 32 || i == 64)) {
			dev_err(&udev->dev, "Invalid ep0 maxpacket: %d\n", i);
			retval = -EMSGSIZE;
			goto fail;
		}
		if (udev->speed == USB_SPEED_FULL)
			dev_dbg(&udev->dev, "ep0 maxpacket = %d\n", i);
		else
			dev_warn(&udev->dev, "Using ep0 maxpacket: %d\n", i);
		udev->ep0.desc.wMaxPacketSize = cpu_to_le16(i);
		usb_ep0_reinit(udev);
	}

	retval = usb_get_device_descriptor(udev, USB_DT_DEVICE_SIZE);
	if (retval < (signed)sizeof(udev->descriptor)) {
		if (retval != -ENODEV)
			dev_err(&udev->dev, "device descriptor read/all, error %d\n",
					retval);
		if (retval >= 0)
			retval = -ENOMSG;
		goto fail;
	}

	usb_detect_quirks(udev);

	if (udev->wusb == 0 && le16_to_cpu(udev->descriptor.bcdUSB) >= 0x0201) {
		retval = usb_get_bos_descriptor(udev);
		if (!retval) {
			udev->lpm_capable = usb_device_supports_lpm(udev);
			usb_set_lpm_parameters(udev);
		}
	}

	retval = 0;
	 
	if (hcd->driver->update_device)
		hcd->driver->update_device(hcd, udev);
	hub_set_initial_usb2_lpm_policy(udev);
fail:
	if (retval) {
		hub_port_disable(hub, port1, 0);
		update_devnum(udev, devnum);	 
	}
	mutex_unlock(hcd->address0_mutex);
	return retval;
}
