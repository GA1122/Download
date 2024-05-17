static int pegasus_blacklisted(struct usb_device *udev)
{
	struct usb_device_descriptor *udd = &udev->descriptor;

	 
	if ((udd->idVendor == cpu_to_le16(VENDOR_BELKIN)) &&
	    (udd->idProduct == cpu_to_le16(0x0121)) &&
	    (udd->bDeviceClass == USB_CLASS_WIRELESS_CONTROLLER) &&
	    (udd->bDeviceProtocol == 1))
		return 1;

	return 0;
}
