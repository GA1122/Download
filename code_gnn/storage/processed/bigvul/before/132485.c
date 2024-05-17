ExtensionFunction::ResponseAction UsbControlTransferFunction::Run() {
  scoped_ptr<extensions::core_api::usb::ControlTransfer::Params> parameters =
      ControlTransfer::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(parameters.get());

  scoped_refptr<UsbDeviceHandle> device_handle =
      GetDeviceHandle(parameters->handle);
  if (!device_handle.get()) {
    return RespondNow(Error(kErrorNoConnection));
  }

  const ControlTransferInfo& transfer = parameters->transfer_info;
  UsbEndpointDirection direction;
  UsbDeviceHandle::TransferRequestType request_type;
  UsbDeviceHandle::TransferRecipient recipient;
  size_t size = 0;

  if (!ConvertDirectionFromApi(transfer.direction, &direction)) {
    return RespondNow(Error(kErrorConvertDirection));
  }

  if (!ConvertRequestTypeFromApi(transfer.request_type, &request_type)) {
    return RespondNow(Error(kErrorConvertRequestType));
  }

  if (!ConvertRecipientFromApi(transfer.recipient, &recipient)) {
    return RespondNow(Error(kErrorConvertRecipient));
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

  device_handle->ControlTransfer(
      direction, request_type, recipient, transfer.request, transfer.value,
      transfer.index, buffer.get(), size, timeout,
      base::Bind(&UsbControlTransferFunction::OnCompleted, this));
  return RespondLater();
}
