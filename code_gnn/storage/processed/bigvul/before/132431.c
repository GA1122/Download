UsbEndpointDirection GetDirection(
    const libusb_endpoint_descriptor* descriptor) {
  switch (descriptor->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK) {
    case LIBUSB_ENDPOINT_IN:
      return USB_DIRECTION_INBOUND;
    case LIBUSB_ENDPOINT_OUT:
      return USB_DIRECTION_OUTBOUND;
    default:
      NOTREACHED();
      return USB_DIRECTION_INBOUND;
  }
}
