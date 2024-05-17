void SafeBrowsingBlockingPageTest::MalwareRedirectCancelAndProceed(
    const std::string open_function) {
  GURL load_url = test_server()->GetURL(
      "files/safe_browsing/interstitial_cancel.html");
  GURL malware_url("http://localhost/files/safe_browsing/malware.html");
  AddURLResult(malware_url, SafeBrowsingService::URL_MALWARE);

  ui_test_utils::NavigateToURL(browser(), load_url);
  ui_test_utils::NavigateToURLWithDisposition(
      browser(),
      GURL("javascript:" + open_function + "()"),
      CURRENT_TAB,
      ui_test_utils::BROWSER_TEST_WAIT_FOR_TAB);
  WaitForInterstitial();
  browser()->ActivateTabAt(0, true);
  ui_test_utils::NavigateToURLWithDisposition(
      browser(),
      GURL("javascript:stopWin()"),
      CURRENT_TAB,
      ui_test_utils::BROWSER_TEST_WAIT_FOR_NAVIGATION);
  browser()->ActivateTabAt(1, true);
  SendCommand("\"proceed\"");
}
