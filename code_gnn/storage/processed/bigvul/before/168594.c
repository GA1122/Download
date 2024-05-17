static int windows_reset_device(struct libusb_device_handle *dev_handle)
{
	struct windows_device_priv *priv = _device_priv(dev_handle->dev);
	return priv->apib->reset_device(SUB_API_NOTSET, dev_handle);
}
