void RenderWidgetHostImpl::IncrementInFlightEventCount() {
  ++in_flight_event_count_;
  if (!is_hidden_)
    StartInputEventAckTimeout(hung_renderer_delay_);
}
