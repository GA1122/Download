  static void ProceedThroughInterstitial(
      SafeBrowsingBlockingPage* sb_interstitial) {
    sb_interstitial->Proceed();
    MessageLoop::current()->RunAllPending();
  }
