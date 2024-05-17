void WebUIBidiCheckerBrowserTest::RunBidiCheckerOnPage(
    const std::string& page_url, bool is_rtl) {
  ui_test_utils::NavigateToURL(browser(), GURL(page_url));
  ASSERT_TRUE(RunJavascriptTest("runBidiChecker",
                                Value::CreateStringValue(page_url),
                                Value::CreateBooleanValue(is_rtl)));
}
