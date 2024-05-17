DownloadHistory* DownloadCoreServiceImpl::GetDownloadHistory() {
  if (!download_manager_created_) {
    GetDownloadManagerDelegate();
  }
  DCHECK(download_manager_created_);
  return download_history_.get();
}
