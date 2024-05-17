void DownloadCoreServiceImpl::CancelDownloads() {
  if (!download_manager_created_)
    return;

  DownloadManager* download_manager =
      BrowserContext::GetDownloadManager(profile_);
  DownloadManager::DownloadVector downloads;
  download_manager->GetAllDownloads(&downloads);
  for (DownloadManager::DownloadVector::iterator it = downloads.begin();
       it != downloads.end(); ++it) {
    if ((*it)->GetState() == content::DownloadItem::IN_PROGRESS)
      (*it)->Cancel(false);
  }
}
