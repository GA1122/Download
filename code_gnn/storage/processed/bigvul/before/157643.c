content::WebContents* ExtensionTabsZoomTest::OpenUrlAndWaitForLoad(
    const GURL& url) {
  ui_test_utils::NavigateToURLWithDisposition(
      browser(), url, WindowOpenDisposition::NEW_FOREGROUND_TAB,
      ui_test_utils::BROWSER_TEST_WAIT_FOR_NAVIGATION);
  return  browser()->tab_strip_model()->GetActiveWebContents();
}
