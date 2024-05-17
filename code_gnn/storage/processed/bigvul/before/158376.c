void RenderWidgetHostImpl::StartInputEventAckTimeout(TimeDelta delay) {
  if (!input_event_ack_timeout_)
    return;
  input_event_ack_timeout_->Start(delay);
  input_event_ack_start_time_ = clock_->NowTicks();
}
