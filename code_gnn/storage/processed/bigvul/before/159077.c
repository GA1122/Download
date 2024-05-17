void ChromeDownloadManagerDelegate::SetDownloadManager(DownloadManager* dm) {
  download_manager_ = dm;

  safe_browsing::SafeBrowsingService* sb_service =
      g_browser_process->safe_browsing_service();
  if (sb_service && !profile_->IsOffTheRecord()) {
    sb_service->AddDownloadManager(dm);
  }
}
