void CheckDownloadComplete(CanDownloadCallback can_download_cb, bool allow) {
  std::move(can_download_cb).Run(true, allow);
}
