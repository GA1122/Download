bool BrowserTabStripController::IsIncognito() {
  return browser_->profile()->IsOffTheRecord();
}
