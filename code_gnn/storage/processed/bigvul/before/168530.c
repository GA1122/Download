static int composite_reset_device(int sub_api, struct libusb_device_handle *dev_handle)
{
	struct windows_device_priv *priv = _device_priv(dev_handle->dev);
	int r;
	uint8_t i; 
	bool available[SUB_API_MAX];
	for (i = 0; i<SUB_API_MAX; i++) {
		available[i] = false;
	}
	for (i=0; i<USB_MAXINTERFACES; i++) {
		if ( (priv->usb_interface[i].apib->id == USB_API_WINUSBX)
		  && (priv->usb_interface[i].sub_api != SUB_API_NOTSET) ) {
			available[priv->usb_interface[i].sub_api] = true;
		}
	}
	for (i=0; i<SUB_API_MAX; i++) {
		if (available[i]) {
			r = usb_api_backend[USB_API_WINUSBX].reset_device(i, dev_handle);
			if (r != LIBUSB_SUCCESS) {
				return r;
			}
		}
	}
	return LIBUSB_SUCCESS;
}
