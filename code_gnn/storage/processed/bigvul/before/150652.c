InputHandlerProxy::EventDisposition InputHandlerProxy::HandleTouchEnd(
    const blink::WebTouchEvent& touch_event) {
  if (touch_event.touches_length == 1)
    touch_result_ = kEventDispositionUndefined;
  return DID_NOT_HANDLE;
}
