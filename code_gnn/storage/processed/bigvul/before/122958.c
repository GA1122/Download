void RenderWidgetHostImpl::ForwardGestureEvent(
    const WebKit::WebGestureEvent& gesture_event) {
  TRACE_EVENT0("renderer_host", "RenderWidgetHostImpl::ForwardGestureEvent");
  if (ignore_input_events_ || process_->IgnoreInputEvents())
    return;

  if (!IsInOverscrollGesture() &&
      !gesture_event_filter_->ShouldForward(gesture_event))
    return;

  ForwardInputEvent(gesture_event, sizeof(WebGestureEvent), false);
}
