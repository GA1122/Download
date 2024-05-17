void DownloadManagerDelegate::GetNextId(const DownloadIdCallback& callback) {
  callback.Run(content::DownloadItem::kInvalidId);
}
