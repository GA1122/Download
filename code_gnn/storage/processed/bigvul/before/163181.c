  void NavigateAndCheckDownload(const GURL& url) {
    const GURL original_url(shell()->web_contents()->GetLastCommittedURL());
    DownloadManager* download_manager = BrowserContext::GetDownloadManager(
        shell()->web_contents()->GetBrowserContext());
    DownloadTestObserverTerminal download_observer(
        download_manager, 1, DownloadTestObserver::ON_DANGEROUS_DOWNLOAD_FAIL);
    NavigateToURL(shell(), url);

    download_observer.WaitForFinished();

    EXPECT_EQ(original_url, shell()->web_contents()->GetLastCommittedURL());
  }
