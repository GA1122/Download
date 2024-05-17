static int composite_abort_control(int sub_api, struct usbi_transfer *itransfer)
{
	struct libusb_transfer *transfer = USBI_TRANSFER_TO_LIBUSB_TRANSFER(itransfer);
	struct windows_transfer_priv *transfer_priv = usbi_transfer_get_os_priv(itransfer);
	struct windows_device_priv *priv = _device_priv(transfer->dev_handle->dev);

	return priv->usb_interface[transfer_priv->interface_number].apib->
		abort_control(priv->usb_interface[transfer_priv->interface_number].sub_api, itransfer);}
