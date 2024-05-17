  static void DontProceedThroughInterstitial(
      SafeBrowsingBlockingPage* sb_interstitial) {
    sb_interstitial->DontProceed();
     MessageLoop::current()->RunAllPending();
   }
