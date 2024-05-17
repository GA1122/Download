std::vector<PageHandler*> PageHandler::ForAgentHost(
    DevToolsAgentHostImpl* host) {
  return host->HandlersByName<PageHandler>(Page::Metainfo::domainName);
}
