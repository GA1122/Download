download::InProgressCache* ChromeDownloadManagerDelegate::GetInProgressCache() {
  DCHECK(download_metadata_cache_ != nullptr);
  return download_metadata_cache_.get();
}
