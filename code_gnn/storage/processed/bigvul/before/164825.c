  void DownloadAndWait(Browser* browser,
                       const GURL& url) {
    DownloadAndWaitWithDisposition(
        browser, url, WindowOpenDisposition::CURRENT_TAB,
        ui_test_utils::BROWSER_TEST_WAIT_FOR_NAVIGATION);
  }
