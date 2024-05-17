  content::DownloadTestObserver* CreateInProgressDownloadObserver(
      size_t download_count) {
    DownloadManager* manager = DownloadManagerForBrowser(browser());
    return new content::DownloadTestObserverInProgress(
        manager, download_count);
  }
