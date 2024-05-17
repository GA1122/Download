static int winusbx_abort_transfers(int sub_api, struct usbi_transfer *itransfer)
{
	struct libusb_transfer *transfer = USBI_TRANSFER_TO_LIBUSB_TRANSFER(itransfer);
	struct libusb_context *ctx = DEVICE_CTX(transfer->dev_handle->dev);
	struct windows_device_handle_priv *handle_priv = _device_handle_priv(transfer->dev_handle);
	struct windows_transfer_priv *transfer_priv = (struct windows_transfer_priv*)usbi_transfer_get_os_priv(itransfer);
	struct windows_device_priv *priv = _device_priv(transfer->dev_handle->dev);
	HANDLE winusb_handle;
	int current_interface;

	CHECK_WINUSBX_AVAILABLE(sub_api);

	current_interface = transfer_priv->interface_number;
	if ((current_interface < 0) || (current_interface >= USB_MAXINTERFACES)) {
		usbi_err(ctx, "program assertion failed: invalid interface_number");
		return LIBUSB_ERROR_NOT_FOUND;
	}
	usbi_dbg("will use interface %d", current_interface);

	winusb_handle = handle_priv->interface_handle[current_interface].api_handle;

	if (!WinUSBX[sub_api].AbortPipe(winusb_handle, transfer->endpoint)) {
		usbi_err(ctx, "AbortPipe failed: %s", windows_error_str(0));
		return LIBUSB_ERROR_NO_DEVICE;
	}

	return LIBUSB_SUCCESS;
}
