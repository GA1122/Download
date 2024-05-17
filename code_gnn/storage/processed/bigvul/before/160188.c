void MemBackendImpl::ModifyStorageSize(int32_t delta) {
  current_size_ += delta;
  if (delta > 0)
    EvictIfNeeded();
}
