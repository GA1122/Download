void DevToolsAgentFilter::OnFilterAdded(IPC::Channel* channel) {
  channel_ = channel;
}
