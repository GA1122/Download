bool DownloadItemImpl::IsInterrupted() const {
  return (state_ == INTERRUPTED);
}
