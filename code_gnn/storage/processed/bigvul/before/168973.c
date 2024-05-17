  void TriggerNavigatorGetPasswordCredentialsAndExpectHasResult(
      content::WebContents* web_contents,
      bool expect_has_results) {
    bool result = false;
    ASSERT_TRUE(content::ExecuteScriptAndExtractBool(
        web_contents,
        "navigator.credentials.get({password: true}).then(c => {"
        "  window.domAutomationController.send(!!c);"
        "});",
        &result));
    ASSERT_EQ(expect_has_results, result);
  }
