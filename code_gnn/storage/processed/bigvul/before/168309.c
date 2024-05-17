bool BrowserView::IsIncognito() const {
  return browser_->profile()->IsOffTheRecord();
}
