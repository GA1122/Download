static int composite_set_interface_altsetting(int sub_api, struct libusb_device_handle *dev_handle, int iface, int altsetting)
{
	struct windows_device_priv *priv = _device_priv(dev_handle->dev);
	return priv->usb_interface[iface].apib->
		set_interface_altsetting(priv->usb_interface[iface].sub_api, dev_handle, iface, altsetting);
}
