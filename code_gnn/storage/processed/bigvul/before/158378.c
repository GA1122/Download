void RenderWidgetHostImpl::StopInputEventAckTimeout() {
  if (input_event_ack_timeout_)
    input_event_ack_timeout_->Stop();

  if (!input_event_ack_start_time_.is_null()) {
    base::TimeDelta elapsed = clock_->NowTicks() - input_event_ack_start_time_;
    const base::TimeDelta kMinimumHangTimeToReport =
        base::TimeDelta::FromSeconds(5);
    if (elapsed >= kMinimumHangTimeToReport)
      UMA_HISTOGRAM_LONG_TIMES("Renderer.Hung.Duration", elapsed);

    input_event_ack_start_time_ = TimeTicks();
  }
  RendererIsResponsive();
}
