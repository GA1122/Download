bool DownloadItemImpl::CanShowInFolder() {
  return !IsCancelled() && !file_externally_removed_;
}
