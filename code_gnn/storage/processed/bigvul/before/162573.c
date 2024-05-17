void Resource::SetDecodedSize(size_t decoded_size) {
  if (decoded_size == decoded_size_)
    return;
  size_t old_size = size();
  decoded_size_ = decoded_size;
  if (IsMainThread())
    GetMemoryCache()->Update(this, old_size, size());
}
