bool BackendImpl::SetMaxSize(int max_bytes) {
  static_assert(sizeof(max_bytes) == sizeof(max_size_),
                "unsupported int model");
  if (max_bytes < 0)
    return false;

  if (!max_bytes)
    return true;

  if (max_bytes >= std::numeric_limits<int32_t>::max() -
                       std::numeric_limits<int32_t>::max() / 10) {
    max_bytes = std::numeric_limits<int32_t>::max() -
                std::numeric_limits<int32_t>::max() / 10 - 1;
  }

  user_flags_ |= kMaxSize;
  max_size_ = max_bytes;
  return true;
}
