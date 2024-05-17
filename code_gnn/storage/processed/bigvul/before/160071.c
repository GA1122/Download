void BackendImpl::OnRead(int32_t bytes) {
  DCHECK_GE(bytes, 0);
  byte_count_ += bytes;
  if (byte_count_ < 0)
    byte_count_ = std::numeric_limits<int32_t>::max();
}
