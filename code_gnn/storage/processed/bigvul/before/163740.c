bool ChromeClientImpl::OpenJavaScriptConfirmDelegate(LocalFrame* frame,
                                                     const String& message) {
  NotifyPopupOpeningObservers();
  WebLocalFrameImpl* webframe = WebLocalFrameImpl::FromFrame(frame);
  if (webframe->Client()) {
    if (WebUserGestureIndicator::IsProcessingUserGesture())
      WebUserGestureIndicator::DisableTimeout();
    return webframe->Client()->RunModalConfirmDialog(message);
  }
  return false;
}
