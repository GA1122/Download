void RenderWidgetHostImpl::RestartHangMonitorTimeout() {
  if (hang_monitor_timeout_)
    hang_monitor_timeout_->Restart(hung_renderer_delay_);
}
