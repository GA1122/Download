void ChromeClientImpl::PrintDelegate(LocalFrame* frame) {
  NotifyPopupOpeningObservers();
  if (web_view_->Client())
    web_view_->Client()->PrintPage(WebLocalFrameImpl::FromFrame(frame));
}
