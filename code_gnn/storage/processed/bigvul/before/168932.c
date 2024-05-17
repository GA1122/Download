bool DevToolsAgentHostImpl::IsAttached() {
  return !sessions_.empty();
}
