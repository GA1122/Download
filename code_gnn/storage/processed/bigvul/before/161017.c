void ChromeClientImpl::TakeFocus(WebFocusType type) {
  if (!web_view_->Client())
    return;
  if (type == kWebFocusTypeBackward)
    web_view_->Client()->FocusPrevious();
  else
    web_view_->Client()->FocusNext();
}
