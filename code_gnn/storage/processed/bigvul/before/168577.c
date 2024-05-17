static int windows_clear_halt(struct libusb_device_handle *dev_handle, unsigned char endpoint)
{
	struct windows_device_priv *priv = _device_priv(dev_handle->dev);
	return priv->apib->clear_halt(SUB_API_NOTSET, dev_handle, endpoint);
}
