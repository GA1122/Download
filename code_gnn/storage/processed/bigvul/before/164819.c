  content::DownloadTestObserver* CreateWaiter(
      Browser* browser, int num_downloads) {
    DownloadManager* download_manager = DownloadManagerForBrowser(browser);
    return new content::DownloadTestObserverTerminal(
        download_manager, num_downloads,
        content::DownloadTestObserver::ON_DANGEROUS_DOWNLOAD_FAIL);
  }
