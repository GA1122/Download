bool WebContentsImpl::ShouldIgnoreUnresponsiveRenderer() {
  return DevToolsAgentHost::IsDebuggerAttached(this);
}
