 static ssize_t k90_store_current_profile(struct device *dev,
					 struct device_attribute *attr,
					 const char *buf, size_t count)
{
	int ret;
	struct usb_interface *usbif = to_usb_interface(dev->parent);
	struct usb_device *usbdev = interface_to_usbdev(usbif);
	int profile;

	if (kstrtoint(buf, 10, &profile))
		return -EINVAL;
	if (profile < 1 || profile > 3)
		return -EINVAL;

	ret = usb_control_msg(usbdev, usb_sndctrlpipe(usbdev, 0),
			      K90_REQUEST_PROFILE,
			      USB_DIR_OUT | USB_TYPE_VENDOR |
			      USB_RECIP_DEVICE, profile, 0, NULL, 0,
			      USB_CTRL_SET_TIMEOUT);
	if (ret != 0) {
		dev_warn(dev, "Failed to change current profile (error %d).\n",
			 ret);
		return ret;
	}

	return count;
}