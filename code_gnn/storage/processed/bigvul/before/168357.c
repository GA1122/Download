bool BrowserView::ShouldShowWindowTitle() const {
#if defined(OS_CHROMEOS)
  if (browser_->is_trusted_source()) {
    return false;
  }
#endif   

  return browser_->SupportsWindowFeature(Browser::FEATURE_TITLEBAR);
}
