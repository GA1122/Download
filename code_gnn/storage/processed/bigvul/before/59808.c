static int hid_post_reset(struct usb_interface *intf)
{
	struct usb_device *dev = interface_to_usbdev (intf);
	struct hid_device *hid = usb_get_intfdata(intf);
	struct usbhid_device *usbhid = hid->driver_data;
	struct usb_host_interface *interface = intf->cur_altsetting;
	int status;
	char *rdesc;

	 
	rdesc = kmalloc(hid->dev_rsize, GFP_KERNEL);
	if (!rdesc)
		return -ENOMEM;

	status = hid_get_class_descriptor(dev,
				interface->desc.bInterfaceNumber,
				HID_DT_REPORT, rdesc, hid->dev_rsize);
	if (status < 0) {
		dbg_hid("reading report descriptor failed (post_reset)\n");
		kfree(rdesc);
		return status;
	}
	status = memcmp(rdesc, hid->dev_rdesc, hid->dev_rsize);
	kfree(rdesc);
	if (status != 0) {
		dbg_hid("report descriptor changed\n");
		return -EPERM;
	}

	 
	spin_lock_irq(&usbhid->lock);
	clear_bit(HID_RESET_PENDING, &usbhid->iofl);
	clear_bit(HID_CLEAR_HALT, &usbhid->iofl);
	spin_unlock_irq(&usbhid->lock);
	hid_set_idle(dev, intf->cur_altsetting->desc.bInterfaceNumber, 0, 0);

	hid_restart_io(hid);

	return 0;
}
