DownloadTestObserverNotInProgress::DownloadTestObserverNotInProgress(
    DownloadManager* download_manager,
    size_t count)
    : DownloadTestObserver(download_manager, count, ON_DANGEROUS_DOWNLOAD_FAIL),
      started_observing_(false) {
  Init();
}
