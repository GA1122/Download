InputEventAckState RenderWidgetHostImpl::FilterInputEvent(
    const blink::WebInputEvent& event, const ui::LatencyInfo& latency_info) {
  if (ShouldDropInputEvents() && event.type != WebInputEvent::TouchCancel)
    return INPUT_EVENT_ACK_STATE_NO_CONSUMER_EXISTS;

  if (!process_->HasConnection())
    return INPUT_EVENT_ACK_STATE_UNKNOWN;

  if (delegate_) {
    if (event.type == WebInputEvent::MouseDown ||
        event.type == WebInputEvent::GestureTapDown ||
        event.type == WebInputEvent::RawKeyDown) {
      delegate_->OnUserInteraction(this, event.type);
    } else if (event.type == WebInputEvent::MouseWheel) {
      if (mouse_wheel_coalesce_timer_->Elapsed().InSecondsF() >
          kMouseWheelCoalesceIntervalInSeconds) {
        delegate_->OnUserInteraction(this, event.type);
      }

      mouse_wheel_coalesce_timer_.reset(new base::ElapsedTimer());
    }
  }

  return view_ ? view_->FilterInputEvent(event)
               : INPUT_EVENT_ACK_STATE_NOT_CONSUMED;
}
