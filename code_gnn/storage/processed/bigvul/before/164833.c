static DownloadManager* DownloadManagerForBrowser(Browser* browser) {
  return BrowserContext::GetDownloadManager(browser->profile());
}
