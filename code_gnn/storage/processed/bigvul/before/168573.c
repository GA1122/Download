static int windows_assign_endpoints(struct libusb_device_handle *dev_handle, int iface, int altsetting)
{
	int i, r;
	struct windows_device_priv *priv = _device_priv(dev_handle->dev);
	struct libusb_config_descriptor *conf_desc;
	const struct libusb_interface_descriptor *if_desc;
	struct libusb_context *ctx = DEVICE_CTX(dev_handle->dev);

	r = libusb_get_config_descriptor(dev_handle->dev, 0, &conf_desc);
	if (r != LIBUSB_SUCCESS) {
		usbi_warn(ctx, "could not read config descriptor: error %d", r);
		return r;
	}

	if (iface >= conf_desc->bNumInterfaces ||
	    altsetting >= conf_desc->interface[iface].num_altsetting) {
		usbi_dbg("interface %d, altsetting %d out of range", iface, altsetting);
		return LIBUSB_ERROR_INVALID_PARAM;
	}

	if_desc = &conf_desc->interface[iface].altsetting[altsetting];
	safe_free(priv->usb_interface[iface].endpoint);

	if (if_desc->bNumEndpoints == 0) {
		usbi_dbg("no endpoints found for interface %d", iface);
		return LIBUSB_SUCCESS;
	}

	priv->usb_interface[iface].endpoint = (uint8_t*) malloc(if_desc->bNumEndpoints);
	if (priv->usb_interface[iface].endpoint == NULL) {
		return LIBUSB_ERROR_NO_MEM;
	}

	priv->usb_interface[iface].nb_endpoints = if_desc->bNumEndpoints;
	for (i=0; i<if_desc->bNumEndpoints; i++) {
		priv->usb_interface[iface].endpoint[i] = if_desc->endpoint[i].bEndpointAddress;
		usbi_dbg("(re)assigned endpoint %02X to interface %d", priv->usb_interface[iface].endpoint[i], iface);
	}
	libusb_free_config_descriptor(conf_desc);

	return priv->apib->configure_endpoints(SUB_API_NOTSET, dev_handle, iface);
}
