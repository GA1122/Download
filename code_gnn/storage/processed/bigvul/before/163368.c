bool RenderThreadImpl::OnMessageReceived(const IPC::Message& msg) {
  if (resource_dispatcher_->OnMessageReceived(msg))
    return true;
  if (file_system_dispatcher_->OnMessageReceived(msg))
    return true;
  return ChildThreadImpl::OnMessageReceived(msg);
}
