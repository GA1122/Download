bool BrowserActionButton::Activate() {
  if (IsPopup()) {
    panel_->OnBrowserActionExecuted(this);


    return false;
  }
  return true;
}
