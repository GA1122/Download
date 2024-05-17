  void TearDownInterstitialPage() {
    interstitial_->DontProceed();
    WaitForInterstitialDetach(shell()->web_contents());
    interstitial_.reset();
  }
