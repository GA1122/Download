void WebLocalFrameImpl::SetDevToolsAgentClient(
    WebDevToolsAgentClient* dev_tools_client) {
  DCHECK(dev_tools_client);
  dev_tools_agent_ = WebDevToolsAgentImpl::Create(this, dev_tools_client);
}
