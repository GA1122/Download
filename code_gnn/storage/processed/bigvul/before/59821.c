static void usbhid_close(struct hid_device *hid)
{
	struct usbhid_device *usbhid = hid->driver_data;

	 
	spin_lock_irq(&usbhid->lock);
	clear_bit(HID_OPENED, &usbhid->iofl);
	if (!(hid->quirks & HID_QUIRK_ALWAYS_POLL))
		clear_bit(HID_IN_POLLING, &usbhid->iofl);
	spin_unlock_irq(&usbhid->lock);

	if (hid->quirks & HID_QUIRK_ALWAYS_POLL)
		return;

	hid_cancel_delayed_stuff(usbhid);
	usb_kill_urb(usbhid->urbin);
	usbhid->intf->needs_remote_wakeup = 0;
}
