bool RenderWidgetHostImpl::Send(IPC::Message* msg) {
  return process_->Send(msg);
}
