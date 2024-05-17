void DownloadItemImpl::OpenDownload() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (IsPartialDownload()) {
    if (!IsTemporary())
      open_when_complete_ = !open_when_complete_;
    return;
  }

  if (!IsComplete() || file_externally_removed_)
    return;

  delegate_->CheckForFileRemoval(this);
  download_stats::RecordOpen(GetEndTime(), !GetOpened());
  opened_ = true;
  FOR_EACH_OBSERVER(Observer, observers_, OnDownloadOpened(this));
  delegate_->DownloadOpened(this);

  if (!open_enabled_)
    return;

  content::GetContentClient()->browser()->OpenItem(GetFullPath());
}
