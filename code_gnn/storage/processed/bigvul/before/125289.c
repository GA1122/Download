void RenderViewImpl::didHandleGestureEvent(const WebGestureEvent& event,
                                           bool event_swallowed) {
#if defined(OS_ANDROID)
  if (event.type == WebInputEvent::GestureTap ||
      event.type == WebInputEvent::GestureLongPress) {
    UpdateTextInputState(SHOW_IME_IF_NEEDED);
  }
#endif
  FOR_EACH_OBSERVER(RenderViewObserver, observers_,
                    DidHandleGestureEvent(event));
}
