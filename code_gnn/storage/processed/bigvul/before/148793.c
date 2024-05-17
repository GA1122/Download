void InterstitialPageImpl::UnderlyingContentObserver::NavigationEntryCommitted(
    const LoadCommittedDetails& load_details) {
  interstitial_->OnNavigatingAwayOrTabClosing();
}
