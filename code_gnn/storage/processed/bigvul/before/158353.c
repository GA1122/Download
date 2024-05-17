void RenderWidgetHostImpl::RestartInputEventAckTimeoutIfNecessary() {
  if (input_event_ack_timeout_ && in_flight_event_count_ > 0 && !is_hidden_)
    input_event_ack_timeout_->Restart(hung_renderer_delay_);
}
