void WebLocalFrameImpl::SetDevToolsAgentImpl(WebDevToolsAgentImpl* agent) {
  DCHECK(!dev_tools_agent_);
  dev_tools_agent_ = agent;
}
