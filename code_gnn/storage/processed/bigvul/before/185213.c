  void BrowserCommandController::RemoveInterstitialObservers(
    TabContents* contents) {
//     WebContents* contents) {
    registrar_.Remove(this, content::NOTIFICATION_INTERSTITIAL_ATTACHED,
                    content::Source<WebContents>(contents->web_contents()));
//                     content::Source<WebContents>(contents));
    registrar_.Remove(this, content::NOTIFICATION_INTERSTITIAL_DETACHED,
                    content::Source<WebContents>(contents->web_contents()));
//                     content::Source<WebContents>(contents));
  }