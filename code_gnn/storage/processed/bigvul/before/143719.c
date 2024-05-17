void RenderWidgetHostImpl::StopHangMonitorTimeout() {
  if (hang_monitor_timeout_)
    hang_monitor_timeout_->Stop();
  RendererIsResponsive();
}
