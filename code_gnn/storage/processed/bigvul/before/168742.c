InputEventAckState RenderWidgetHostImpl::FilterInputEvent(
    const blink::WebInputEvent& event, const ui::LatencyInfo& latency_info) {
  if (ShouldDropInputEvents() && event.GetType() != WebInputEvent::kTouchCancel)
    return INPUT_EVENT_ACK_STATE_NO_CONSUMER_EXISTS;

  if (!process_->HasConnection())
    return INPUT_EVENT_ACK_STATE_UNKNOWN;

  if (delegate_) {
    if (event.GetType() == WebInputEvent::kMouseDown ||
        event.GetType() == WebInputEvent::kTouchStart) {
      delegate_->FocusOwningWebContents(this);
    }
    if (event.GetType() == WebInputEvent::kMouseDown ||
        event.GetType() == WebInputEvent::kGestureScrollBegin ||
        event.GetType() == WebInputEvent::kTouchStart ||
        event.GetType() == WebInputEvent::kRawKeyDown) {
      delegate_->OnUserInteraction(this, event.GetType());
    }
  }

  return view_ ? view_->FilterInputEvent(event)
               : INPUT_EVENT_ACK_STATE_NOT_CONSUMED;
}
