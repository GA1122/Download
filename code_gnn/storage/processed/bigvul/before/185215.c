  void BrowserCommandController::TabReplacedAt(TabStripModel* tab_strip_model,
                                               TabContents* old_contents,
                                               TabContents* new_contents,
                                               int index) {
  RemoveInterstitialObservers(old_contents);
//   RemoveInterstitialObservers(old_contents->web_contents());
    AddInterstitialObservers(new_contents->web_contents());
  }