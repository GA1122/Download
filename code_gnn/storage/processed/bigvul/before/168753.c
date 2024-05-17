void RenderWidgetHostImpl::IncrementInFlightEventCount() {
  ++in_flight_event_count_;
  if (!is_hidden_)
    StartHangMonitorTimeout(hung_renderer_delay_);
}
