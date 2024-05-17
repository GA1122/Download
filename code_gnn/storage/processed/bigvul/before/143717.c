void RenderWidgetHostImpl::StartHangMonitorTimeout(base::TimeDelta delay) {
  if (hang_monitor_timeout_)
    hang_monitor_timeout_->Start(delay);
}
