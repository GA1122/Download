void BrowserCommandController::TabInsertedAt(WebContents* contents,
                                             int index,
                                             bool foreground) {
   AddInterstitialObservers(contents);
 }
