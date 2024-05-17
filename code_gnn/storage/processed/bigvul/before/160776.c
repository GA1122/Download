void RenderViewImpl::DidHandleGestureEvent(const WebGestureEvent& event,
                                           bool event_cancelled) {
  RenderWidget::DidHandleGestureEvent(event, event_cancelled);

  if (!event_cancelled) {
    for (auto& observer : observers_)
      observer.DidHandleGestureEvent(event);
  }
}
