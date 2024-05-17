bool ConvertRequestTypeFromApi(const RequestType& input,
                               UsbDeviceHandle::TransferRequestType* output) {
  switch (input) {
    case usb::REQUEST_TYPE_STANDARD:
      *output = UsbDeviceHandle::STANDARD;
      return true;
    case usb::REQUEST_TYPE_CLASS:
      *output = UsbDeviceHandle::CLASS;
      return true;
    case usb::REQUEST_TYPE_VENDOR:
      *output = UsbDeviceHandle::VENDOR;
      return true;
    case usb::REQUEST_TYPE_RESERVED:
      *output = UsbDeviceHandle::RESERVED;
      return true;
    default:
      NOTREACHED();
      return false;
  }
}
