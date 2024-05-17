bool DownloadItemImpl::CanOpenDownload() {
  return !file_externally_removed_;
}
