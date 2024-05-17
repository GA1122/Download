 static int composite_claim_interface(int sub_api, struct libusb_device_handle *dev_handle, int iface)
{
	struct windows_device_priv *priv = _device_priv(dev_handle->dev);
	return priv->usb_interface[iface].apib->
		claim_interface(priv->usb_interface[iface].sub_api, dev_handle, iface);
}
