void RenderViewImpl::DidHandleGestureEventForWidget(
    const WebGestureEvent& event) {
  for (auto& observer : observers_)
    observer.DidHandleGestureEvent(event);
}
