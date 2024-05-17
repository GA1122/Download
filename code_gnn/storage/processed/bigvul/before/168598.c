static void windows_transfer_callback(struct usbi_transfer *itransfer, uint32_t io_result, uint32_t io_size)
{
	struct libusb_transfer *transfer = USBI_TRANSFER_TO_LIBUSB_TRANSFER(itransfer);
	struct windows_device_priv *priv = _device_priv(transfer->dev_handle->dev);
	int status, istatus;

	usbi_dbg("handling I/O completion with errcode %d, size %d", io_result, io_size);

	switch(io_result) {
	case NO_ERROR:
		status = priv->apib->copy_transfer_data(SUB_API_NOTSET, itransfer, io_size);
		break;
	case ERROR_GEN_FAILURE:
		usbi_dbg("detected endpoint stall");
		status = LIBUSB_TRANSFER_STALL;
		break;
	case ERROR_SEM_TIMEOUT:
		usbi_dbg("detected semaphore timeout");
		status = LIBUSB_TRANSFER_TIMED_OUT;
		break;
	case ERROR_OPERATION_ABORTED:
		istatus = priv->apib->copy_transfer_data(SUB_API_NOTSET, itransfer, io_size);
		if (istatus != LIBUSB_TRANSFER_COMPLETED) {
			usbi_dbg("Failed to copy partial data in aborted operation: %d", istatus);
		}
		if (itransfer->flags & USBI_TRANSFER_TIMED_OUT) {
			usbi_dbg("detected timeout");
			status = LIBUSB_TRANSFER_TIMED_OUT;
		} else {
			usbi_dbg("detected operation aborted");
			status = LIBUSB_TRANSFER_CANCELLED;
		}
		break;
	default:
		usbi_err(ITRANSFER_CTX(itransfer), "detected I/O error %d: %s", io_result, windows_error_str(0));
		status = LIBUSB_TRANSFER_ERROR;
		break;
	}
	windows_clear_transfer_priv(itransfer);	 
	usbi_handle_transfer_completion(itransfer, (enum libusb_transfer_status)status);
}
