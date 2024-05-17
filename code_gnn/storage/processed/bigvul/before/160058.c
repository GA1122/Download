bool BackendImpl::IsAllocAllowed(int current_size, int new_size) {
  DCHECK_GT(new_size, current_size);
  if (user_flags_ & kNoBuffering)
    return false;

  int to_add = new_size - current_size;
  if (buffer_bytes_ + to_add > MaxBuffersSize())
    return false;

  buffer_bytes_ += to_add;
  CACHE_UMA(COUNTS_50000, "BufferBytes", 0, buffer_bytes_ / 1024);
  return true;
}
