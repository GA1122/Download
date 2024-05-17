static int usbhid_open(struct hid_device *hid)
{
	struct usbhid_device *usbhid = hid->driver_data;
	int res;

	set_bit(HID_OPENED, &usbhid->iofl);

	if (hid->quirks & HID_QUIRK_ALWAYS_POLL)
		return 0;

	res = usb_autopm_get_interface(usbhid->intf);
	 
	if (res < 0) {
		clear_bit(HID_OPENED, &usbhid->iofl);
		return -EIO;
	}

	usbhid->intf->needs_remote_wakeup = 1;

	set_bit(HID_RESUME_RUNNING, &usbhid->iofl);
	set_bit(HID_IN_POLLING, &usbhid->iofl);

	res = hid_start_in(hid);
	if (res) {
		if (res != -ENOSPC) {
			hid_io_error(hid);
			res = 0;
		} else {
			 
			res = -EBUSY;
			clear_bit(HID_OPENED, &usbhid->iofl);
			clear_bit(HID_IN_POLLING, &usbhid->iofl);
			usbhid->intf->needs_remote_wakeup = 0;
		}
	}

	usb_autopm_put_interface(usbhid->intf);

	 
	if (res == 0)
		msleep(50);

	clear_bit(HID_RESUME_RUNNING, &usbhid->iofl);
	return res;
}
