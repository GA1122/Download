void RenderWidgetHostImpl::IncrementInFlightEventCount() {
  increment_in_flight_event_count();
  if (!is_hidden_)
    StartHangMonitorTimeout(hung_renderer_delay_);
}
