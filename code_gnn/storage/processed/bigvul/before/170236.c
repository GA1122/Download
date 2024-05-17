  bool IsShowingInterstitial(content::WebContents* tab) {
    if (AreCommittedInterstitialsEnabled()) {
      security_interstitials::SecurityInterstitialTabHelper* helper =
          security_interstitials::SecurityInterstitialTabHelper::
              FromWebContents(tab);
      if (!helper) {
        return false;
      }
      return helper
                 ->GetBlockingPageForCurrentlyCommittedNavigationForTesting() !=
             nullptr;
    }
    return tab->GetInterstitialPage() != nullptr;
  }
