IntRect ChromeClientImpl::RootWindowRect() {
  WebRect rect;
  if (web_view_->Client()) {
    rect = web_view_->Client()->RootWindowRect();
  } else {
    rect.width = web_view_->Size().width;
    rect.height = web_view_->Size().height;
  }
  return IntRect(rect);
}
