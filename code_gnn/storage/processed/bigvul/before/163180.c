  void ExecuteScriptAndCheckWindowOpenDownload(RenderFrameHost* rfh,
                                               const std::string& javascript) {
    const GURL original_url(shell()->web_contents()->GetLastCommittedURL());
    ShellAddedObserver new_shell_observer;
    DownloadManager* download_manager = BrowserContext::GetDownloadManager(
        shell()->web_contents()->GetBrowserContext());

    EXPECT_TRUE(ExecuteScript(rfh, javascript));
    Shell* new_shell = new_shell_observer.GetShell();

    DownloadTestObserverTerminal download_observer(
        download_manager, 1, DownloadTestObserver::ON_DANGEROUS_DOWNLOAD_FAIL);

    WaitForLoadStop(new_shell->web_contents());
    download_observer.WaitForFinished();

    EXPECT_TRUE(
        new_shell->web_contents()->GetLastCommittedURL().spec().empty());
    EXPECT_FALSE(
        new_shell->web_contents()->GetController().GetLastCommittedEntry());
    EXPECT_EQ(original_url, shell()->web_contents()->GetLastCommittedURL());
  }
