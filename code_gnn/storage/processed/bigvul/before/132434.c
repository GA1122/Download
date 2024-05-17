UsbUsageType GetUsageType(const libusb_endpoint_descriptor* descriptor) {
  switch ((descriptor->bmAttributes & LIBUSB_ISO_USAGE_TYPE_MASK) >> 4) {
    case LIBUSB_ISO_USAGE_TYPE_DATA:
      return USB_USAGE_DATA;
    case LIBUSB_ISO_USAGE_TYPE_FEEDBACK:
      return USB_USAGE_FEEDBACK;
    case LIBUSB_ISO_USAGE_TYPE_IMPLICIT:
      return USB_USAGE_EXPLICIT_FEEDBACK;
    default:
      NOTREACHED();
      return USB_USAGE_DATA;
  }
}
