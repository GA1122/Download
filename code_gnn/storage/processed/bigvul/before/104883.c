size_t PPB_URLLoader_Impl::FillUserBuffer() {
  DCHECK(user_buffer_);
  DCHECK(user_buffer_size_);

  size_t bytes_to_copy = std::min(buffer_.size(), user_buffer_size_);
  std::copy(buffer_.begin(), buffer_.begin() + bytes_to_copy, user_buffer_);
  buffer_.erase(buffer_.begin(), buffer_.begin() + bytes_to_copy);

  DCHECK(!is_asynchronous_load_suspended_ || request_info_);
  if (is_asynchronous_load_suspended_ &&
      buffer_.size() <= static_cast<size_t>(
          request_info_->prefetch_buffer_lower_threshold())) {
    DVLOG(1) << "Resuming async load - buffer size: " << buffer_.size();
    loader_->setDefersLoading(false);
    is_asynchronous_load_suspended_ = false;
  }

  user_buffer_ = NULL;
  user_buffer_size_ = 0;
  return bytes_to_copy;
}
