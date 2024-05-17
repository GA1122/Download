void RenderWidgetHostImpl::ForwardMouseEvent(const WebMouseEvent& mouse_event) {
  TRACE_EVENT2("renderer_host", "RenderWidgetHostImpl::ForwardMouseEvent",
               "x", mouse_event.x, "y", mouse_event.y);
  if (ignore_input_events_ || process_->IgnoreInputEvents())
    return;

  if (CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kSimulateTouchScreenWithMouse)) {
    SimulateTouchGestureWithMouse(mouse_event);
    return;
  }

  if (mouse_event.type == WebInputEvent::MouseDown &&
      gesture_event_filter_->GetTapSuppressionController()->
          ShouldDeferMouseDown(mouse_event))
      return;
  if (mouse_event.type == WebInputEvent::MouseUp &&
      gesture_event_filter_->GetTapSuppressionController()->
          ShouldSuppressMouseUp())
      return;

  ForwardMouseEventImmediately(mouse_event);
}
