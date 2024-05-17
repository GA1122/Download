bool BrowserView::ShouldShowWindowIcon() const {
#if defined(USE_ASH)
  if (browser_->is_app() && browser_->app_type() == Browser::APP_TYPE_HOST)
    return false;
#endif
  return browser_->SupportsWindowFeature(Browser::FEATURE_TITLEBAR);
}
