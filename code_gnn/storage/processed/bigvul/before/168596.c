static int windows_set_interface_altsetting(struct libusb_device_handle *dev_handle, int iface, int altsetting)
{
	int r = LIBUSB_SUCCESS;
	struct windows_device_priv *priv = _device_priv(dev_handle->dev);

	safe_free(priv->usb_interface[iface].endpoint);
	priv->usb_interface[iface].nb_endpoints= 0;

	r = priv->apib->set_interface_altsetting(SUB_API_NOTSET, dev_handle, iface, altsetting);

	if (r == LIBUSB_SUCCESS) {
		r = windows_assign_endpoints(dev_handle, iface, altsetting);
	}

	return r;
}
