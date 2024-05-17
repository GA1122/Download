void RenderWidgetHostImpl::ForwardWheelEvent(
    const WebMouseWheelEvent& wheel_event) {
  TRACE_EVENT0("renderer_host", "RenderWidgetHostImpl::ForwardWheelEvent");
  if (ignore_input_events_ || process_->IgnoreInputEvents())
    return;

  if (mouse_wheel_pending_) {
    if (coalesced_mouse_wheel_events_.empty() ||
        !ShouldCoalesceMouseWheelEvents(coalesced_mouse_wheel_events_.back(),
                                        wheel_event)) {
      coalesced_mouse_wheel_events_.push_back(wheel_event);
    } else {
      WebMouseWheelEvent* last_wheel_event =
          &coalesced_mouse_wheel_events_.back();
      last_wheel_event->deltaX += wheel_event.deltaX;
      last_wheel_event->deltaY += wheel_event.deltaY;
      last_wheel_event->wheelTicksX += wheel_event.wheelTicksX;
      last_wheel_event->wheelTicksY += wheel_event.wheelTicksY;
      DCHECK_GE(wheel_event.timeStampSeconds,
                last_wheel_event->timeStampSeconds);
      last_wheel_event->timeStampSeconds = wheel_event.timeStampSeconds;
    }
    return;
  }
  mouse_wheel_pending_ = true;
  current_wheel_event_ = wheel_event;

  HISTOGRAM_COUNTS_100("MPArch.RWH_WheelQueueSize",
                       coalesced_mouse_wheel_events_.size());

  ForwardInputEvent(wheel_event, sizeof(WebMouseWheelEvent), false);
}
