  void FocusFirstNameField() {
    bool result = false;
    ASSERT_TRUE(content::ExecuteScriptAndExtractBool(
        GetRenderViewHost(),
        "if (document.readyState === 'complete')"
        "  document.getElementById('firstname').focus();"
        "else"
        "  domAutomationController.send(false);",
        &result));
    ASSERT_TRUE(result);
  }
