  ~TestInterstitialPageStateGuard() {
    if (interstitial_page_)
      interstitial_page_->ClearStates();
  }
