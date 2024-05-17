int usb_authorize_device(struct usb_device *usb_dev)
{
	int result = 0, c;

	usb_lock_device(usb_dev);
	if (usb_dev->authorized == 1)
		goto out_authorized;

	result = usb_autoresume_device(usb_dev);
	if (result < 0) {
		dev_err(&usb_dev->dev,
			"can't autoresume for authorization: %d\n", result);
		goto error_autoresume;
	}

	if (usb_dev->wusb) {
		result = usb_get_device_descriptor(usb_dev, sizeof(usb_dev->descriptor));
		if (result < 0) {
			dev_err(&usb_dev->dev, "can't re-read device descriptor for "
				"authorization: %d\n", result);
			goto error_device_descriptor;
		}
	}

	usb_dev->authorized = 1;
	 
	c = usb_choose_configuration(usb_dev);
	if (c >= 0) {
		result = usb_set_configuration(usb_dev, c);
		if (result) {
			dev_err(&usb_dev->dev,
				"can't set config #%d, error %d\n", c, result);
			 
		}
	}
	dev_info(&usb_dev->dev, "authorized to connect\n");

error_device_descriptor:
	usb_autosuspend_device(usb_dev);
error_autoresume:
out_authorized:
	usb_unlock_device(usb_dev);	 
	return result;
}
