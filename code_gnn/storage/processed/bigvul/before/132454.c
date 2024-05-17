TransferType ConvertTransferTypeToApi(const UsbTransferType& input) {
  switch (input) {
    case device::USB_TRANSFER_CONTROL:
      return usb::TRANSFER_TYPE_CONTROL;
    case device::USB_TRANSFER_INTERRUPT:
      return usb::TRANSFER_TYPE_INTERRUPT;
    case device::USB_TRANSFER_ISOCHRONOUS:
      return usb::TRANSFER_TYPE_ISOCHRONOUS;
    case device::USB_TRANSFER_BULK:
      return usb::TRANSFER_TYPE_BULK;
    default:
      NOTREACHED();
      return usb::TRANSFER_TYPE_NONE;
  }
}
