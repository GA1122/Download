SynchronizationType ConvertSynchronizationTypeToApi(
    const UsbSynchronizationType& input) {
  switch (input) {
    case device::USB_SYNCHRONIZATION_NONE:
      return usb::SYNCHRONIZATION_TYPE_NONE;
    case device::USB_SYNCHRONIZATION_ASYNCHRONOUS:
      return usb::SYNCHRONIZATION_TYPE_ASYNCHRONOUS;
    case device::USB_SYNCHRONIZATION_ADAPTIVE:
      return usb::SYNCHRONIZATION_TYPE_ADAPTIVE;
    case device::USB_SYNCHRONIZATION_SYNCHRONOUS:
      return usb::SYNCHRONIZATION_TYPE_SYNCHRONOUS;
    default:
      NOTREACHED();
      return usb::SYNCHRONIZATION_TYPE_NONE;
  }
}
