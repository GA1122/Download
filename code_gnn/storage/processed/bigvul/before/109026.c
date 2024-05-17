bool RenderViewImpl::Send(IPC::Message* message) {
  return RenderWidget::Send(message);
}
