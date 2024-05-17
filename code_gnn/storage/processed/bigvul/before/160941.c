void ChromeClientImpl::CloseWindowSoon() {
  if (web_view_->Client())
    web_view_->Client()->CloseWidgetSoon();
}
