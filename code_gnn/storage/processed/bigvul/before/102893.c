void TabCloseableStateWatcher::CheckAndUpdateState(
    const Browser* browser_to_check) {
  if (waiting_for_browser_ || signing_off_ || tabstrip_watchers_.empty() ||
      (browser_to_check && browser_to_check->type() != Browser::TYPE_NORMAL))
    return;

  bool new_can_close;

  if (tabstrip_watchers_.size() > 1) {
    new_can_close = true;
  } else {   
    if (!browser_to_check)
      browser_to_check = tabstrip_watchers_[0]->browser();
    if (browser_to_check->profile()->IsOffTheRecord() && !guest_session_) {
      new_can_close = true;
    } else {
      TabStripModel* tabstrip_model = browser_to_check->tabstrip_model();
      if (tabstrip_model->count() == 1) {
        new_can_close =
            tabstrip_model->GetTabContentsAt(0)->tab_contents()->GetURL() !=
                GURL(chrome::kChromeUINewTabURL);   
      } else {
        new_can_close = true;
      }
    }
  }

  SetCloseableState(new_can_close);
}
