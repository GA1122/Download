void RenderViewImpl::OnDiscardInputEvent(
    const blink::WebInputEvent* input_event,
    const std::vector<const blink::WebInputEvent*>& coalesced_events,
    const ui::LatencyInfo& latency_info,
    InputEventDispatchType dispatch_type) {
  if (!input_event || dispatch_type == DISPATCH_TYPE_NON_BLOCKING) {
    return;
  }

  InputEventAck ack(InputEventAckSource::MAIN_THREAD, input_event->GetType(),
                    INPUT_EVENT_ACK_STATE_NOT_CONSUMED);
  Send(new InputHostMsg_HandleInputEvent_ACK(routing_id_, ack));
}
