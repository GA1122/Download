  explicit TestInterstitialPageStateGuard(
      TestInterstitialPage* interstitial_page)
      : interstitial_page_(interstitial_page) {
    DCHECK(interstitial_page_);
    interstitial_page_->set_delegate(this);
  }
