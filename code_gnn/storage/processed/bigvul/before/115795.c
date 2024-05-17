  void ProceedThroughInterstitial() {
    TabContents* contents = browser()->GetSelectedTabContents();
    InterstitialPage* interstitial_page = InterstitialPage::GetInterstitialPage(
        contents);
    ASSERT_TRUE(interstitial_page);
    interstitial_page->Proceed();
  }
