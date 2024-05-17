  static void CheckSafeSearch(Browser* browser, bool expect_safe_search) {
    content::WebContents* web_contents =
        browser->tab_strip_model()->GetActiveWebContents();
    content::TestNavigationObserver observer(web_contents);
    LocationBar* location_bar = browser->window()->GetLocationBar();
    ui_test_utils::SendToOmniboxAndSubmit(location_bar, "http://google.com/");
    OmniboxEditModel* model = location_bar->GetOmniboxView()->model();
    observer.Wait();
    EXPECT_TRUE(model->CurrentMatch(NULL).destination_url.is_valid());
    EXPECT_EQ(GetExpectedSearchURL(expect_safe_search), web_contents->GetURL());
  }
