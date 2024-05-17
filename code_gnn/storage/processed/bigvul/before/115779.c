  void AssertNoInterstitial(bool wait_for_delete) {
    TabContents* contents = browser()->GetSelectedTabContents();

    if (contents->showing_interstitial_page() && wait_for_delete) {
      static_cast<TestSafeBrowsingBlockingPage*>(
          contents->interstitial_page())->set_wait_for_delete();
      ui_test_utils::RunMessageLoop();
    }

    ASSERT_FALSE(contents->showing_interstitial_page());
  }
