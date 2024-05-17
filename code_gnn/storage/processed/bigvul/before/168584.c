static int windows_get_active_config_descriptor(struct libusb_device *dev, unsigned char *buffer, size_t len, int *host_endian)
{
	struct windows_device_priv *priv = _device_priv(dev);

	if (priv->active_config == 0)
		return LIBUSB_ERROR_NOT_FOUND;

	return windows_get_config_descriptor(dev, (uint8_t)(priv->active_config-1), buffer, len, host_endian);
}
