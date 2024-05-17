std::vector<SecurityHandler*> SecurityHandler::ForAgentHost(
    DevToolsAgentHostImpl* host) {
  return DevToolsSession::HandlersForAgentHost<SecurityHandler>(
      host, Security::Metainfo::domainName);
}
