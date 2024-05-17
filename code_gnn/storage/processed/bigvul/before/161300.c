std::vector<InspectorHandler*> InspectorHandler::ForAgentHost(
    DevToolsAgentHostImpl* host) {
  return DevToolsSession::HandlersForAgentHost<InspectorHandler>(
      host, Inspector::Metainfo::domainName);
}
