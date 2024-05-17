InputEventAckState RenderWidgetHostImpl::FilterInputEvent(
    const blink::WebInputEvent& event, const ui::LatencyInfo& latency_info) {
  if (ShouldDropInputEvents() && event.GetType() != WebInputEvent::kTouchCancel)
    return INPUT_EVENT_ACK_STATE_NO_CONSUMER_EXISTS;

  if (!process_->IsInitializedAndNotDead())
    return INPUT_EVENT_ACK_STATE_UNKNOWN;

  if (delegate_) {
    if (event.GetType() == WebInputEvent::kMouseDown ||
        event.GetType() == WebInputEvent::kTouchStart ||
        event.GetType() == WebInputEvent::kGestureTap) {
      delegate_->FocusOwningWebContents(this);
    }
    delegate_->DidReceiveInputEvent(this, event.GetType());
  }

  return view_ ? view_->FilterInputEvent(event)
               : INPUT_EVENT_ACK_STATE_NOT_CONSUMED;
}
