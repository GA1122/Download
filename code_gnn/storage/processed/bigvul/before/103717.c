void DevToolsAgentFilter::OnDebuggerCommand(const std::string& command) {
  WebDevToolsAgent::executeDebuggerCommand(
      WebString::fromUTF8(command), current_routing_id_);
}
