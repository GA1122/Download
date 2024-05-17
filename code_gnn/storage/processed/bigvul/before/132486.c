ExtensionFunction::ResponseAction UsbBulkTransferFunction::Run() {
  scoped_ptr<extensions::core_api::usb::BulkTransfer::Params> parameters =
      BulkTransfer::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(parameters.get());

  scoped_refptr<UsbDeviceHandle> device_handle =
      GetDeviceHandle(parameters->handle);
  if (!device_handle.get()) {
    return RespondNow(Error(kErrorNoConnection));
  }

  const GenericTransferInfo& transfer = parameters->transfer_info;
  UsbEndpointDirection direction;
  size_t size = 0;

  if (!ConvertDirectionFromApi(transfer.direction, &direction)) {
    return RespondNow(Error(kErrorConvertDirection));
  }

  if (!GetTransferSize(transfer, &size)) {
    return RespondNow(Error(kErrorInvalidTransferLength));
  }

  scoped_refptr<net::IOBuffer> buffer =
      CreateBufferForTransfer(transfer, direction, size);
  if (!buffer.get()) {
    return RespondNow(Error(kErrorMalformedParameters));
  }

  int timeout = transfer.timeout ? *transfer.timeout : 0;
  if (timeout < 0) {
    return RespondNow(Error(kErrorInvalidTimeout));
  }

  device_handle->BulkTransfer(
      direction, transfer.endpoint, buffer.get(), size, timeout,
      base::Bind(&UsbBulkTransferFunction::OnCompleted, this));
  return RespondLater();
}
