void BrowserChildProcessHostImpl::OnChannelInitialized(IPC::Channel* channel) {
  channel_ = channel;
}
