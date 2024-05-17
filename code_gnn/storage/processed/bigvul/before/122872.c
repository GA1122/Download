IPC::ChannelProxy* RenderProcessHostImpl::GetChannel() {
  return channel_.get();
}
