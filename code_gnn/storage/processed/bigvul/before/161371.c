std::vector<PageHandler*> PageHandler::ForAgentHost(
    DevToolsAgentHostImpl* host) {
  return DevToolsSession::HandlersForAgentHost<PageHandler>(
       host, Page::Metainfo::domainName);
 }
