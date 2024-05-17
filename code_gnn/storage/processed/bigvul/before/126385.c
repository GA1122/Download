bool BrowserWindowGtk::IsTabStripSupported() const {
  return browser_->SupportsWindowFeature(Browser::FEATURE_TABSTRIP);
}
