testing::AssertionResult CheckStyleInjection(Browser* browser,
                                             const GURL& url,
                                             bool expected_injection) {
  ui_test_utils::NavigateToURL(browser, url);

  bool css_injected = false;
  if (!content::ExecuteScriptAndExtractBool(
          browser->tab_strip_model()->GetActiveWebContents(),
          "window.domAutomationController.send("
          "    document.defaultView.getComputedStyle(document.body, null)."
          "        getPropertyValue('display') == 'none');",
          &css_injected)) {
    return testing::AssertionFailure()
        << "Failed to execute script and extract bool for injection status.";
  }

  if (css_injected != expected_injection) {
    std::string message;
    if (css_injected)
      message = "CSS injected when no injection was expected.";
    else
      message = "CSS not injected when injection was expected.";
    return testing::AssertionFailure() << message;
  }

  bool css_doesnt_add_to_list = false;
  if (!content::ExecuteScriptAndExtractBool(
          browser->tab_strip_model()->GetActiveWebContents(),
          "window.domAutomationController.send("
          "    document.styleSheets.length == 0);",
          &css_doesnt_add_to_list)) {
    return testing::AssertionFailure()
        << "Failed to execute script and extract bool for stylesheets length.";
  }
  if (!css_doesnt_add_to_list) {
    return testing::AssertionFailure()
        << "CSS injection added to number of stylesheets.";
  }

  return testing::AssertionSuccess();
}
