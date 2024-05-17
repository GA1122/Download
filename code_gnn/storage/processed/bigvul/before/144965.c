void RenderWidgetHostViewAura::ProcessAckedTouchEvent(
    const TouchEventWithLatencyInfo& touch,
    InputEventAckState ack_result) {
  aura::WindowTreeHost* host = window_->GetHost();
  if (!host)
    return;

  DCHECK(touch.event.GetType() != blink::WebInputEvent::kTouchScrollStarted);

  ui::EventResult result = (ack_result == INPUT_EVENT_ACK_STATE_CONSUMED)
                               ? ui::ER_HANDLED
                               : ui::ER_UNHANDLED;

  blink::WebTouchPoint::State required_state;
  switch (touch.event.GetType()) {
    case blink::WebInputEvent::kTouchStart:
      required_state = blink::WebTouchPoint::kStatePressed;
      break;
    case blink::WebInputEvent::kTouchEnd:
      required_state = blink::WebTouchPoint::kStateReleased;
      break;
    case blink::WebInputEvent::kTouchMove:
      required_state = blink::WebTouchPoint::kStateMoved;
      break;
    case blink::WebInputEvent::kTouchCancel:
      required_state = blink::WebTouchPoint::kStateCancelled;
      break;
    default:
      required_state = blink::WebTouchPoint::kStateUndefined;
      NOTREACHED();
      break;
  }

  bool sent_ack = false;
  for (size_t i = 0; i < touch.event.touches_length; ++i) {
    if (touch.event.touches[i].state == required_state) {
      DCHECK(!sent_ack);
      host->dispatcher()->ProcessedTouchEvent(touch.event.unique_touch_event_id,
                                              window_, result);
      sent_ack = true;
    }
  }
}
