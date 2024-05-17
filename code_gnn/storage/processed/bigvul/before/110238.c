BufferSizeStatus GetBufferStatus(const char* data, size_t len) {
  if (len < sizeof(NaClIPCAdapter::NaClMessageHeader))
    return MESSAGE_IS_TRUNCATED;

  const NaClIPCAdapter::NaClMessageHeader* header =
      reinterpret_cast<const NaClIPCAdapter::NaClMessageHeader*>(data);
  uint32 message_size =
      sizeof(NaClIPCAdapter::NaClMessageHeader) + header->payload_size;

  if (len == message_size)
    return MESSAGE_IS_COMPLETE;
  if (len > message_size)
    return MESSAGE_HAS_EXTRA_DATA;
  return MESSAGE_IS_TRUNCATED;
}
