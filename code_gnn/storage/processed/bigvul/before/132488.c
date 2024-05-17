ExtensionFunction::ResponseAction UsbIsochronousTransferFunction::Run() {
  scoped_ptr<extensions::core_api::usb::IsochronousTransfer::Params>
      parameters = IsochronousTransfer::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(parameters.get());

  scoped_refptr<UsbDeviceHandle> device_handle =
      GetDeviceHandle(parameters->handle);
  if (!device_handle.get()) {
    return RespondNow(Error(kErrorNoConnection));
  }

  const IsochronousTransferInfo& transfer = parameters->transfer_info;
  const GenericTransferInfo& generic_transfer = transfer.transfer_info;
  size_t size = 0;
  UsbEndpointDirection direction;

  if (!ConvertDirectionFromApi(generic_transfer.direction, &direction)) {
    return RespondNow(Error(kErrorConvertDirection));
  }

  if (!GetTransferSize(generic_transfer, &size)) {
    return RespondNow(Error(kErrorInvalidTransferLength));
  }

  if (transfer.packets < 0 || transfer.packets >= kMaxPackets) {
    return RespondNow(Error(kErrorInvalidNumberOfPackets));
  }

  unsigned int packets = transfer.packets;
  if (transfer.packet_length < 0 ||
      transfer.packet_length >= kMaxPacketLength) {
    return RespondNow(Error(kErrorInvalidPacketLength));
  }

  unsigned int packet_length = transfer.packet_length;
  const uint64 total_length = packets * packet_length;
  if (packets > size || total_length > size) {
    return RespondNow(Error(kErrorTransferLength));
  }

  scoped_refptr<net::IOBuffer> buffer =
      CreateBufferForTransfer(generic_transfer, direction, size);
  if (!buffer.get()) {
    return RespondNow(Error(kErrorMalformedParameters));
  }

  int timeout = generic_transfer.timeout ? *generic_transfer.timeout : 0;
  if (timeout < 0) {
    return RespondNow(Error(kErrorInvalidTimeout));
  }

  device_handle->IsochronousTransfer(
      direction, generic_transfer.endpoint, buffer.get(), size, packets,
      packet_length, timeout,
      base::Bind(&UsbIsochronousTransferFunction::OnCompleted, this));
  return RespondLater();
}
