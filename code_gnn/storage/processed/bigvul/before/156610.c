  void VerifyBrowsingInstanceExpectations(content::WebContents* main_contents,
                                          content::WebContents* new_contents) {
    std::string location_of_opened_window;
    EXPECT_TRUE(ExecuteScriptAndExtractString(
        new_contents,
        "w = window.open('', 'main_contents');"
        "window.domAutomationController.send(w.location.href);",
        &location_of_opened_window));
    EXPECT_EQ(url::kAboutBlankURL, location_of_opened_window);
  }
