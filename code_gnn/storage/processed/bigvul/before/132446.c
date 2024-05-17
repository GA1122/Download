bool ConvertDirectionFromApi(const Direction& input,
                             UsbEndpointDirection* output) {
  switch (input) {
    case usb::DIRECTION_IN:
      *output = device::USB_DIRECTION_INBOUND;
      return true;
    case usb::DIRECTION_OUT:
      *output = device::USB_DIRECTION_OUTBOUND;
      return true;
    default:
      NOTREACHED();
      return false;
  }
}
