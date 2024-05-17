void RenderWidgetHostImpl::DecrementInFlightEventCount(
    InputEventAckSource ack_source) {
  --in_flight_event_count_;
  if (in_flight_event_count_ <= 0) {
    StopInputEventAckTimeout();
  } else {
    if (ack_source == InputEventAckSource::MAIN_THREAD)
      RestartInputEventAckTimeoutIfNecessary();
  }
}
