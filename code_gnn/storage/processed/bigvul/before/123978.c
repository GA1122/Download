void RenderViewImpl::didHandleGestureEvent(
    const WebGestureEvent& event,
    bool event_cancelled) {
  RenderWidget::didHandleGestureEvent(event, event_cancelled);
  FOR_EACH_OBSERVER(RenderViewObserver, observers_,
                    DidHandleGestureEvent(event));
}
