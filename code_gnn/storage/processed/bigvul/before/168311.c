bool BrowserView::IsTabStripVisible() const {
  if (!browser_->SupportsWindowFeature(Browser::FEATURE_TABSTRIP))
    return false;

  return tabstrip_ != nullptr;
}
