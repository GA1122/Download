void DevToolsUIBindings::DispatchProtocolMessageFromDevToolsFrontend(
    const std::string& message) {
  if (agent_host_.get())
    agent_host_->DispatchProtocolMessage(this, message);
}
