base::Optional<download::DownloadEntry> DownloadManagerImpl::GetInProgressEntry(
    download::DownloadItemImpl* download) {
  return in_progress_manager_->GetInProgressEntry(download);
}
