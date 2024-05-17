void RenderViewImpl::Show(WebNavigationPolicy policy) {
  if (did_show_ && !webkit_preferences_.supports_multiple_windows) {
    return;
  }

  RenderWidget::Show(policy);
}
