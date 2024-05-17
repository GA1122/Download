static int usbhid_restart_ctrl_queue(struct usbhid_device *usbhid)
{
	struct hid_device *hid = usb_get_intfdata(usbhid->intf);
	int kicked;
	int r;

	WARN_ON(hid == NULL);
	if (!hid || test_bit(HID_RESET_PENDING, &usbhid->iofl) ||
			test_bit(HID_SUSPENDED, &usbhid->iofl))
		return 0;

	if ((kicked = (usbhid->ctrlhead != usbhid->ctrltail))) {
		hid_dbg(hid, "Kicking head %d tail %d", usbhid->ctrlhead, usbhid->ctrltail);

		 
		r = usb_autopm_get_interface_async(usbhid->intf);
		if (r < 0)
			return r;

		 
		if (test_bit(HID_SUSPENDED, &usbhid->iofl)) {
			usb_autopm_put_interface_no_suspend(usbhid->intf);
			return r;
		}

		 
		set_bit(HID_CTRL_RUNNING, &usbhid->iofl);
		if (hid_submit_ctrl(hid)) {
			clear_bit(HID_CTRL_RUNNING, &usbhid->iofl);
			usb_autopm_put_interface_async(usbhid->intf);
		}
		wake_up(&usbhid->wait);
	}
	return kicked;
}