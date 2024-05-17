bool ConvertRecipientFromApi(const Recipient& input,
                             UsbDeviceHandle::TransferRecipient* output) {
  switch (input) {
    case usb::RECIPIENT_DEVICE:
      *output = UsbDeviceHandle::DEVICE;
      return true;
    case usb::RECIPIENT_INTERFACE:
      *output = UsbDeviceHandle::INTERFACE;
      return true;
    case usb::RECIPIENT_ENDPOINT:
      *output = UsbDeviceHandle::ENDPOINT;
      return true;
    case usb::RECIPIENT_OTHER:
      *output = UsbDeviceHandle::OTHER;
      return true;
    default:
      NOTREACHED();
      return false;
  }
}
