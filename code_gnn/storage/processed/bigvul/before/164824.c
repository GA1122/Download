  explicit DisableSafeBrowsingOnInProgressDownload(Browser* browser)
      : DownloadTestObserver(DownloadManagerForBrowser(browser),
                             1,
                             ON_DANGEROUS_DOWNLOAD_QUIT),
        browser_(browser),
        final_state_seen_(false) {
    Init();
  }
