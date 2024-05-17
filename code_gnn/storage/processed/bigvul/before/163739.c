bool ChromeClientImpl::OpenJavaScriptAlertDelegate(LocalFrame* frame,
                                                   const String& message) {
  NotifyPopupOpeningObservers();
  WebLocalFrameImpl* webframe = WebLocalFrameImpl::FromFrame(frame);
  if (webframe->Client()) {
    if (WebUserGestureIndicator::IsProcessingUserGesture())
      WebUserGestureIndicator::DisableTimeout();
    webframe->Client()->RunModalAlertDialog(message);
    return true;
  }
  return false;
}
