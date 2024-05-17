static int interface_by_endpoint(struct windows_device_priv *priv,
	struct windows_device_handle_priv *handle_priv, uint8_t endpoint_address)
{
	int i, j;
	for (i=0; i<USB_MAXINTERFACES; i++) {
		if (handle_priv->interface_handle[i].api_handle == INVALID_HANDLE_VALUE)
			continue;
		if (handle_priv->interface_handle[i].api_handle == 0)
			continue;
		if (priv->usb_interface[i].endpoint == NULL)
			continue;
		for (j=0; j<priv->usb_interface[i].nb_endpoints; j++) {
			if (priv->usb_interface[i].endpoint[j] == endpoint_address) {
				return i;
			}
		}
	}
	return -1;
}
