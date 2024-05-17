void RenderWidgetHostImpl::RestartHangMonitorTimeout() {
  time_when_considered_hung_ = Time();
  StartHangMonitorTimeout(
      TimeDelta::FromMilliseconds(hung_renderer_delay_ms_));
}
