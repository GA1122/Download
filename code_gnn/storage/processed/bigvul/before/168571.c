static int windows_abort_control(struct usbi_transfer *itransfer)
{
	struct libusb_transfer *transfer = USBI_TRANSFER_TO_LIBUSB_TRANSFER(itransfer);
	struct windows_device_priv *priv = _device_priv(transfer->dev_handle->dev);

	return priv->apib->abort_control(SUB_API_NOTSET, itransfer);
}
