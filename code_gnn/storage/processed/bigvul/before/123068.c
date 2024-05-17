void RenderWidgetHostImpl::StopHangMonitorTimeout() {
  time_when_considered_hung_ = Time();
  RendererIsResponsive();
}
