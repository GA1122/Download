  content::DownloadTestObserver* CreateInProgressWaiter(
      Browser* browser, int num_downloads) {
    DownloadManager* download_manager = DownloadManagerForBrowser(browser);
    return new content::DownloadTestObserverInProgress(
        download_manager, num_downloads);
  }
