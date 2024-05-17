static int windows_release_interface(struct libusb_device_handle *dev_handle, int iface)
{
	struct windows_device_priv *priv = _device_priv(dev_handle->dev);

	return priv->apib->release_interface(SUB_API_NOTSET, dev_handle, iface);
}
