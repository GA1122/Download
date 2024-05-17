int DownloadItemImpl::PercentComplete() const {
  if (delegate_delayed_complete_ || total_bytes_ <= 0)
    return -1;

  return static_cast<int>(GetReceivedBytes() * 100.0 / total_bytes_);
}
