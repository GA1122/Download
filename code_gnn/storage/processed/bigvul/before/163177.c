  void ExecuteScriptAndCheckNavigationDownload(RenderFrameHost* rfh,
                                               const std::string& javascript) {
    const GURL original_url(shell()->web_contents()->GetLastCommittedURL());
    DownloadManager* download_manager = BrowserContext::GetDownloadManager(
        shell()->web_contents()->GetBrowserContext());
    DownloadTestObserverTerminal download_observer(
        download_manager, 1, DownloadTestObserver::ON_DANGEROUS_DOWNLOAD_FAIL);

    EXPECT_TRUE(ExecuteScript(rfh, javascript));
    download_observer.WaitForFinished();

    EXPECT_EQ(original_url, shell()->web_contents()->GetLastCommittedURL());
  }
