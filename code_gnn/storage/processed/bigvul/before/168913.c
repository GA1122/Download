bool DevToolsAgentHostImpl::DispatchProtocolMessage(
    DevToolsAgentHostClient* client,
    const std::string& message) {
  DevToolsSession* session = SessionByClient(client);
  if (!session)
    return false;
  DispatchProtocolMessage(session, message);
  return true;
}
