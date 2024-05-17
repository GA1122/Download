size_t UrlmonUrlRequest::SendDataToDelegate(size_t bytes_to_read) {
  DCHECK_EQ(thread_, base::PlatformThread::CurrentId());
  DCHECK_NE(id(), -1);
  DCHECK_GT(bytes_to_read, 0U);
  size_t bytes_copied = 0;
  if (delegate_) {
    std::string read_data;
    if (cache_) {
      HRESULT hr = ReadStream(cache_, bytes_to_read, &read_data);
      if (hr == S_FALSE || read_data.length() < bytes_to_read) {
        DVLOG(1) << __FUNCTION__ << me() << "all cached data read";
        cache_.Release();
      }
    }

    if (read_data.empty() && pending_data_) {
      size_t pending_data_read_save = pending_read_size_;
      pending_read_size_ = 0;

      base::win::ScopedComPtr<IStream> pending(pending_data_);
      HRESULT hr = ReadStream(pending, bytes_to_read, &read_data);
      if (read_data.empty())
        pending_read_size_ = pending_data_read_save;
      if (hr == S_FALSE)
        pending_data_.Release();
    }

    bytes_copied = read_data.length();

    if (bytes_copied) {
      ++calling_delegate_;
      DCHECK_NE(id(), -1);
      if (delegate_)
        delegate_->OnReadComplete(id(), read_data);
      --calling_delegate_;
    }
  } else {
    DLOG(ERROR) << __FUNCTION__ << me() << "no delegate";
  }

  return bytes_copied;
}
