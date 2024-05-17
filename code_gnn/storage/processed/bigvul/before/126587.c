void TabStripGtk::TabBlockedStateChanged(WebContents* contents, int index) {
  GetTabAt(index)->SetBlocked(model_->IsTabBlocked(index));
}
