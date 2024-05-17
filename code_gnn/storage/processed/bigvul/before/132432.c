UsbSynchronizationType GetSynchronizationType(
    const libusb_endpoint_descriptor* descriptor) {
  switch ((descriptor->bmAttributes & LIBUSB_ISO_SYNC_TYPE_MASK) >> 2) {
    case LIBUSB_ISO_SYNC_TYPE_NONE:
      return USB_SYNCHRONIZATION_NONE;
    case LIBUSB_ISO_SYNC_TYPE_ASYNC:
      return USB_SYNCHRONIZATION_ASYNCHRONOUS;
    case LIBUSB_ISO_SYNC_TYPE_ADAPTIVE:
      return USB_SYNCHRONIZATION_ADAPTIVE;
    case LIBUSB_ISO_SYNC_TYPE_SYNC:
      return USB_SYNCHRONIZATION_SYNCHRONOUS;
    default:
      NOTREACHED();
      return USB_SYNCHRONIZATION_NONE;
  }
}
