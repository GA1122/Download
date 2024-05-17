void ChromeClientImpl::ShowUnhandledTapUIIfNeeded(WebTappedInfo& tapped_info) {
  if (web_view_->Client()) {
    web_view_->Client()->ShowUnhandledTapUIIfNeeded(tapped_info);
  }
}
