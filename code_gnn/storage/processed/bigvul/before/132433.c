UsbTransferType GetTransferType(const libusb_endpoint_descriptor* descriptor) {
  switch (descriptor->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK) {
    case LIBUSB_TRANSFER_TYPE_CONTROL:
      return USB_TRANSFER_CONTROL;
    case LIBUSB_TRANSFER_TYPE_ISOCHRONOUS:
      return USB_TRANSFER_ISOCHRONOUS;
    case LIBUSB_TRANSFER_TYPE_BULK:
      return USB_TRANSFER_BULK;
    case LIBUSB_TRANSFER_TYPE_INTERRUPT:
      return USB_TRANSFER_INTERRUPT;
    default:
      NOTREACHED();
      return USB_TRANSFER_CONTROL;
  }
}
