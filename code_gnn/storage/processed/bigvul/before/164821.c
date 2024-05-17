  content::DownloadTestObserver* DangerousDownloadWaiter(
      Browser* browser,
      int num_downloads,
      content::DownloadTestObserver::DangerousDownloadAction
          dangerous_download_action) {
    DownloadManager* download_manager = DownloadManagerForBrowser(browser);
    return new content::DownloadTestObserverTerminal(
        download_manager, num_downloads, dangerous_download_action);
  }
