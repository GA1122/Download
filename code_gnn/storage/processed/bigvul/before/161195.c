  void JSExpect(const std::string& expression) {
    bool result;
    EXPECT_TRUE(content::ExecuteScriptAndExtractBool(
        browser()->tab_strip_model()->GetActiveWebContents(),
        "window.domAutomationController.send(!!(" + expression + "));",
        &result));
    EXPECT_TRUE(result) << expression;
  }
