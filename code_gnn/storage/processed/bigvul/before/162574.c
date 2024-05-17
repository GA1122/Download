void Resource::SetEncodedSize(size_t encoded_size) {
  if (encoded_size == encoded_size_ &&
      encoded_size == encoded_size_memory_usage_)
    return;
  size_t old_size = size();
  encoded_size_ = encoded_size;
  encoded_size_memory_usage_ = encoded_size;
  if (IsMainThread())
    GetMemoryCache()->Update(this, old_size, size());
}
