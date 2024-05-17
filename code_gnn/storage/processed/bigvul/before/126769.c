SkColor BrowserView::GetInfoBarSeparatorColor() const {
  return (IsTabStripVisible() || !frame_->ShouldUseNativeFrame()) ?
      ThemeService::GetDefaultColor(ThemeService::COLOR_TOOLBAR_SEPARATOR) :
      SK_ColorBLACK;
}
