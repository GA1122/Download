bool ChromeClientImpl::OpenJavaScriptPromptDelegate(LocalFrame* frame,
                                                    const String& message,
                                                    const String& default_value,
                                                    String& result) {
  NotifyPopupOpeningObservers();
  WebLocalFrameImpl* webframe = WebLocalFrameImpl::FromFrame(frame);
  if (webframe->Client()) {
    if (WebUserGestureIndicator::IsProcessingUserGesture(webframe))
      WebUserGestureIndicator::DisableTimeout();
    WebString actual_value;
    bool ok = webframe->Client()->RunModalPromptDialog(message, default_value,
                                                       &actual_value);
    if (ok)
      result = actual_value;
    return ok;
  }
  return false;
}
