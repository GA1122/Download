void ChromeDownloadManagerDelegate::OnCheckDownloadAllowedComplete(
    content::CheckDownloadAllowedCallback check_download_allowed_cb,
    bool storage_permission_granted,
    bool allow) {
  if (!storage_permission_granted) {
  } else if (allow) {
    RecordDownloadSource(DOWNLOAD_INITIATED_BY_NAVIGATION);
  } else {
    RecordDownloadCount(CHROME_DOWNLOAD_COUNT_BLOCKED_BY_THROTTLING);
  }

  std::move(check_download_allowed_cb).Run(allow);
}
