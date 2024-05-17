void DevToolsDownloadManagerDelegate::GetNextId(
    const content::DownloadIdCallback& callback) {
  static uint32_t next_id = content::DownloadItem::kInvalidId + 1;
  if (proxy_download_delegate_) {
    proxy_download_delegate_->GetNextId(callback);
    return;
  }
  callback.Run(next_id++);
}
