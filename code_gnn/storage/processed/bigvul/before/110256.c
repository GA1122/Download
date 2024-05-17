void NaClIPCAdapter::RewrittenMessage::SetData(
    const NaClIPCAdapter::NaClMessageHeader& header,
    const void* payload,
    size_t payload_length) {
  DCHECK(!data_.get() && data_len_ == 0);
  size_t header_len = sizeof(NaClIPCAdapter::NaClMessageHeader);
  data_len_ = header_len + payload_length;
  data_.reset(new char[data_len_]);

  memcpy(data_.get(), &header, sizeof(NaClIPCAdapter::NaClMessageHeader));
  memcpy(&data_[header_len], payload, payload_length);
}
