  void ProceedThroughInterstitial(TabContents* tab) {
    InterstitialPage* interstitial_page = tab->interstitial_page();
    ASSERT_TRUE(interstitial_page);
    interstitial_page->Proceed();
    ui_test_utils::WaitForNavigation(&(tab->controller()));
  }
