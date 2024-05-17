void InterstitialPageImpl::UnderlyingContentObserver::WebContentsDestroyed() {
  interstitial_->OnNavigatingAwayOrTabClosing();
}
