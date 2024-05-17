bool WebContentsImpl::PreHandleWheelEvent(
    const blink::WebMouseWheelEvent& event) {
#if !defined(OS_MACOSX)
  if (delegate_ &&
      event.wheelTicksY &&
      (event.modifiers & blink::WebInputEvent::ControlKey)) {
    delegate_->ContentsZoomChange(event.wheelTicksY > 0);
    return true;
  }
#endif

  return false;
}
