void ChromeDownloadManagerDelegate::SetDownloadManager(DownloadManager* dm) {
  download_manager_ = dm;
  download_history_.reset(new DownloadHistory(profile_));
  download_history_->Load(
      base::Bind(&DownloadManager::OnPersistentStoreQueryComplete,
                 base::Unretained(dm)));
  extension_event_router_.reset(new ExtensionDownloadsEventRouter(
      profile_, download_manager_));
}
