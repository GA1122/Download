void ChromeDownloadManagerDelegate::GetNextId(
    const content::DownloadIdCallback& callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (profile_->IsOffTheRecord()) {
    content::BrowserContext::GetDownloadManager(
        profile_->GetOriginalProfile())->GetDelegate()->GetNextId(callback);
    return;
  }
  if (next_download_id_ == content::DownloadItem::kInvalidId) {
    id_callbacks_.push_back(callback);
    return;
  }
  ReturnNextId(callback);
}
