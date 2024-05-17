  void NavigateToURLImpl(const GURL& dest_url) const {
    EXPECT_TRUE(prerender_manager()->FindEntry(dest_url_) != NULL);

    ui_test_utils::NavigateToURL(browser(), dest_url);

    EXPECT_TRUE(prerender_manager()->FindEntry(dest_url_) == NULL);

    if (call_javascript_) {
      bool display_test_result = false;
      ASSERT_TRUE(ui_test_utils::ExecuteJavaScriptAndExtractBool(
          browser()->GetSelectedTabContents()->render_view_host(), L"",
          L"window.domAutomationController.send(DidDisplayPass())",
          &display_test_result));
      EXPECT_TRUE(display_test_result);
    }
  }
