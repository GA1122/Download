bool MemBackendImpl::HasExceededStorageSize() const {
  return current_size_ > max_size_;
}
