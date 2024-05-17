void Browser::CheckDownloadsInProgress(bool* normal_downloads_are_present,
                                       bool* incognito_downloads_are_present) {
  *normal_downloads_are_present = false;
  *incognito_downloads_are_present = false;

  DownloadManager* download_manager = NULL;
  if (profile()->HasCreatedDownloadManager())
    download_manager = profile()->GetDownloadManager();
  if (profile()->IsOffTheRecord()) {
    *incognito_downloads_are_present =
        (download_manager && download_manager->in_progress_count() != 0);
    if (profile()->GetOriginalProfile()->HasCreatedDownloadManager())
      download_manager = profile()->GetOriginalProfile()->GetDownloadManager();
  }

  *normal_downloads_are_present =
      (download_manager && download_manager->in_progress_count() != 0);
}
