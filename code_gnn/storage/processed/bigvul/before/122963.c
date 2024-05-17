void RenderWidgetHostImpl::ForwardMouseEventImmediately(
    const WebMouseEvent& mouse_event) {
  TRACE_EVENT2("renderer_host",
               "RenderWidgetHostImpl::ForwardMouseEventImmediately",
               "x", mouse_event.x, "y", mouse_event.y);
  if (ignore_input_events_ || process_->IgnoreInputEvents())
    return;

  if (CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kSimulateTouchScreenWithMouse)) {
    SimulateTouchGestureWithMouse(mouse_event);
    return;
  }

  if (mouse_event.type == WebInputEvent::MouseMove) {
    if (mouse_move_pending_) {
      if (!next_mouse_move_.get()) {
        next_mouse_move_.reset(new WebMouseEvent(mouse_event));
      } else {
        int x = next_mouse_move_->movementX;
        int y = next_mouse_move_->movementY;
        *next_mouse_move_ = mouse_event;
        next_mouse_move_->movementX += x;
        next_mouse_move_->movementY += y;
      }
      return;
    }
    mouse_move_pending_ = true;
  } else if (mouse_event.type == WebInputEvent::MouseDown) {
    OnUserGesture();
  }

  ForwardInputEvent(mouse_event, sizeof(WebMouseEvent), false);
}
