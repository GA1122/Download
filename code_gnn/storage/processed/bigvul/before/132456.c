scoped_refptr<net::IOBuffer> CreateBufferForTransfer(
    const T& input,
    UsbEndpointDirection direction,
    size_t size) {
  if (size >= kMaxTransferLength)
    return NULL;

  scoped_refptr<net::IOBuffer> buffer =
      new net::IOBuffer(std::max(static_cast<size_t>(1), size));

  if (direction == device::USB_DIRECTION_INBOUND) {
    return buffer;
  } else if (direction == device::USB_DIRECTION_OUTBOUND) {
    if (input.data.get() && size <= input.data->size()) {
      memcpy(buffer->data(), input.data->data(), size);
      return buffer;
    }
  }
  NOTREACHED();
  return NULL;
}
