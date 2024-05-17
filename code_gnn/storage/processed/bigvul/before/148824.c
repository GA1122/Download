  explicit ClipboardMessageWatcher(InterstitialPage* interstitial) {
    interstitial->GetMainFrame()->GetProcess()->GetChannel()->AddFilter(this);
  }
