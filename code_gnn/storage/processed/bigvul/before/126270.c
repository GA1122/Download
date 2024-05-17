void BrowserCommandController::AddInterstitialObservers(WebContents* contents) {
  registrar_.Add(this, content::NOTIFICATION_INTERSTITIAL_ATTACHED,
                 content::Source<WebContents>(contents));
  registrar_.Add(this, content::NOTIFICATION_INTERSTITIAL_DETACHED,
                 content::Source<WebContents>(contents));
 }
