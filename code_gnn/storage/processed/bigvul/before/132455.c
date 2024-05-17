UsageType ConvertUsageTypeToApi(const UsbUsageType& input) {
  switch (input) {
    case device::USB_USAGE_DATA:
      return usb::USAGE_TYPE_DATA;
    case device::USB_USAGE_FEEDBACK:
      return usb::USAGE_TYPE_FEEDBACK;
    case device::USB_USAGE_EXPLICIT_FEEDBACK:
      return usb::USAGE_TYPE_EXPLICITFEEDBACK;
    default:
      NOTREACHED();
      return usb::USAGE_TYPE_NONE;
  }
}
