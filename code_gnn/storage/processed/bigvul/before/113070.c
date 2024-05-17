bool DownloadItemImpl::IsCancelled() const {
  return (state_ == CANCELLED) ||
         (state_ == INTERRUPTED);
}
