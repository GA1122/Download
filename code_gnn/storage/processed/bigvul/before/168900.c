void BrowserDevToolsAgentHost::DispatchProtocolMessage(
    DevToolsSession* session,
    const std::string& message) {
  session->DispatchProtocolMessage(message);
}
