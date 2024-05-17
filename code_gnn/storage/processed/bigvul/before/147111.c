void WebLocalFrameImpl::WillBeDetached() {
  if (dev_tools_agent_)
    dev_tools_agent_->WillBeDestroyed();
}
