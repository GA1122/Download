  void WaitForInterstitial(content::WebContents* tab) {
    if (!AreCommittedInterstitialsEnabled()) {
      content::WaitForInterstitialAttach(tab);
      ASSERT_TRUE(IsShowingInterstitial(tab));
      ASSERT_TRUE(
          WaitForRenderFrameReady(tab->GetInterstitialPage()->GetMainFrame()));
    } else {
      ASSERT_TRUE(IsShowingInterstitial(tab));
      ASSERT_TRUE(WaitForRenderFrameReady(tab->GetMainFrame()));
    }
  }
