static int windows_claim_interface(struct libusb_device_handle *dev_handle, int iface)
{
	int r = LIBUSB_SUCCESS;
	struct windows_device_priv *priv = _device_priv(dev_handle->dev);

	if (iface >= USB_MAXINTERFACES)
		return LIBUSB_ERROR_INVALID_PARAM;

	safe_free(priv->usb_interface[iface].endpoint);
	priv->usb_interface[iface].nb_endpoints= 0;

	r = priv->apib->claim_interface(SUB_API_NOTSET, dev_handle, iface);

	if (r == LIBUSB_SUCCESS) {
		r = windows_assign_endpoints(dev_handle, iface, 0);
	}

	return r;
}
