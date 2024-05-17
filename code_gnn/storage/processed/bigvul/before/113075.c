bool DownloadItemImpl::IsPartialDownload() const {
  return (state_ == IN_PROGRESS);
}
