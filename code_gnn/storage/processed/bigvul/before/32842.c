static int wdm_manage_power(struct usb_interface *intf, int on)
{
	 
	int rv = usb_autopm_get_interface(intf);
	if (rv < 0)
		goto err;

	intf->needs_remote_wakeup = on;
	usb_autopm_put_interface(intf);
err:
	return rv;
}
