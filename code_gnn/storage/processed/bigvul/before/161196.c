  void JSExpectAsync(const std::string& function) {
    bool result;
    EXPECT_TRUE(content::ExecuteScriptAndExtractBool(
        browser()->tab_strip_model()->GetActiveWebContents(),
        "(" + function + ")(function() {"
        "  window.domAutomationController.send(true);"
        "});",
        &result));
    EXPECT_TRUE(result);
  }
