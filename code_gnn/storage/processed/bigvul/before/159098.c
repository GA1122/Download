void DownloadItemImpl::DelayedDownloadOpened(bool auto_opened) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  auto_opened_ = auto_opened;
  Completed();
}
