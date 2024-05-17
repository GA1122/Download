DownloadId ChromeDownloadManagerDelegate::GetNextId() {
  if (!profile_->IsOffTheRecord())
    return DownloadId(this, next_download_id_++);

  return BrowserContext::GetDownloadManager(profile_->GetOriginalProfile())->
      GetDelegate()->GetNextId();
}
