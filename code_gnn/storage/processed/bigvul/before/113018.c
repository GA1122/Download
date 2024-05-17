void DownloadItemImpl::DelayedDownloadOpened(bool auto_opened) {
  auto_opened_ = auto_opened;
  Completed();
}
