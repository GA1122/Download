static int windows_open(struct libusb_device_handle *dev_handle)
{
	struct windows_device_priv *priv = _device_priv(dev_handle->dev);
	struct libusb_context *ctx = DEVICE_CTX(dev_handle->dev);

	if (priv->apib == NULL) {
		usbi_err(ctx, "program assertion failed - device is not initialized");
		return LIBUSB_ERROR_NO_DEVICE;
	}

	return priv->apib->open(SUB_API_NOTSET, dev_handle);
}
