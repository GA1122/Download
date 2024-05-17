void HeadlessWebContentsImpl::DevToolsAgentHostDetached(
    content::DevToolsAgentHost* agent_host) {
  for (auto& observer : observers_)
    observer.DevToolsClientDetached();
}
