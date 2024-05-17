const char* ConvertTransferStatusToApi(const UsbTransferStatus status) {
  switch (status) {
    case device::USB_TRANSFER_COMPLETED:
      return "";
    case device::USB_TRANSFER_ERROR:
      return kErrorGeneric;
    case device::USB_TRANSFER_TIMEOUT:
      return kErrorTimeout;
    case device::USB_TRANSFER_CANCELLED:
      return kErrorCancelled;
    case device::USB_TRANSFER_STALLED:
      return kErrorStalled;
    case device::USB_TRANSFER_DISCONNECT:
      return kErrorDisconnect;
    case device::USB_TRANSFER_OVERFLOW:
      return kErrorOverflow;
    case device::USB_TRANSFER_LENGTH_SHORT:
      return kErrorTransferLength;
    default:
      NOTREACHED();
      return "";
  }
}
