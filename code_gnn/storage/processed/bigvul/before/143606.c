void RenderWidgetHostImpl::DecrementInFlightEventCount() {
  if (decrement_in_flight_event_count() <= 0) {
    StopHangMonitorTimeout();
  } else {
    if (!is_hidden_)
      RestartHangMonitorTimeout();
  }
}
