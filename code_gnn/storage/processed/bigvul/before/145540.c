  explicit SecurityExploitTestInterstitialPage(WebContents* contents) {
    InterstitialPage* interstitial = InterstitialPage::Create(
        contents, true, contents->GetLastCommittedURL(), this);
    interstitial->Show();
  }
