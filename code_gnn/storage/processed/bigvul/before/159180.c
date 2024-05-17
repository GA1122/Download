void DownloadItemImpl::OpenDownload() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (!IsDone()) {
    if (!IsTemporary())
      open_when_complete_ = !open_when_complete_;
    return;
  }

  if (state_ != COMPLETE_INTERNAL || file_externally_removed_)
    return;

  delegate_->CheckForFileRemoval(this);
  RecordOpen(GetEndTime(), !GetOpened());
  opened_ = true;
  last_access_time_ = base::Time::Now();
  for (auto& observer : observers_)
    observer.OnDownloadOpened(this);
  delegate_->OpenDownload(this);
}
