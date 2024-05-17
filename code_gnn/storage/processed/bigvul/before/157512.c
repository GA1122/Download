void VerifyProxyScript(Browser* browser) {
  ui_test_utils::NavigateToURL(browser, GURL("http://google.com"));

  bool result = false;
  EXPECT_TRUE(content::ExecuteScriptAndExtractBool(
      browser->tab_strip_model()->GetActiveWebContents(),
      "var textContent = document.body.textContent;"
      "var hasError = textContent.indexOf('ERR_PROXY_CONNECTION_FAILED') >= 0;"
      "domAutomationController.send(hasError);",
      &result));
  EXPECT_TRUE(result);
}
