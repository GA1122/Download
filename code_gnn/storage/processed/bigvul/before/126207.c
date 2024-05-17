Browser::DownloadClosePreventionType Browser::OkToCloseWithInProgressDownloads(
    int* num_downloads_blocking) const {
  DCHECK(num_downloads_blocking);
  *num_downloads_blocking = 0;

  if (IsAttemptingToCloseBrowser())
    return DOWNLOAD_CLOSE_OK;

  if (!g_browser_process->profile_manager())
    return DOWNLOAD_CLOSE_OK;

  int total_download_count = DownloadService::DownloadCountAllProfiles();
  if (total_download_count == 0)
    return DOWNLOAD_CLOSE_OK;    

  int profile_window_count = 0;
  int total_window_count = 0;
  for (BrowserList::const_iterator iter = BrowserList::begin();
       iter != BrowserList::end(); ++iter) {
    Browser* const browser = *iter;
    if (browser == this || browser->IsAttemptingToCloseBrowser())
      continue;

    if ((*iter)->profile() == profile())
      profile_window_count++;
    total_window_count++;
  }

  if (total_window_count == 0) {
    *num_downloads_blocking = total_download_count;
    return DOWNLOAD_CLOSE_BROWSER_SHUTDOWN;
  }

  DownloadService* download_service =
      DownloadServiceFactory::GetForProfile(profile());
  if (profile_window_count == 0 && download_service->DownloadCount() > 0 &&
      profile()->IsOffTheRecord()) {
    *num_downloads_blocking = download_service->DownloadCount();
    return DOWNLOAD_CLOSE_LAST_WINDOW_IN_INCOGNITO_PROFILE;
  }

  return DOWNLOAD_CLOSE_OK;
}
