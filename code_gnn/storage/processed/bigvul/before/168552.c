static int submit_bulk_transfer(struct usbi_transfer *itransfer)
{
	struct libusb_transfer *transfer = USBI_TRANSFER_TO_LIBUSB_TRANSFER(itransfer);
	struct libusb_context *ctx = DEVICE_CTX(transfer->dev_handle->dev);
	struct windows_transfer_priv *transfer_priv = (struct windows_transfer_priv*)usbi_transfer_get_os_priv(itransfer);
	struct windows_device_priv *priv = _device_priv(transfer->dev_handle->dev);
	int r;

	r = priv->apib->submit_bulk_transfer(SUB_API_NOTSET, itransfer);
	if (r != LIBUSB_SUCCESS) {
		return r;
	}

	usbi_add_pollfd(ctx, transfer_priv->pollable_fd.fd,
		(short)(IS_XFERIN(transfer) ? POLLIN : POLLOUT));

	itransfer->flags |= USBI_TRANSFER_UPDATED_FDS;
	return LIBUSB_SUCCESS;
}
