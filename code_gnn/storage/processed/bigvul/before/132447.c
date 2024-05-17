Direction ConvertDirectionToApi(const UsbEndpointDirection& input) {
  switch (input) {
    case device::USB_DIRECTION_INBOUND:
      return usb::DIRECTION_IN;
    case device::USB_DIRECTION_OUTBOUND:
      return usb::DIRECTION_OUT;
    default:
      NOTREACHED();
      return usb::DIRECTION_NONE;
  }
}
