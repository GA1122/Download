DownloadItem* DownloadManagerImpl::GetDownload(uint32_t download_id) {
  return base::ContainsKey(downloads_, download_id)
             ? downloads_[download_id].get()
             : nullptr;
}
