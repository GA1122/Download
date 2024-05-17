std::vector<InputHandler*> InputHandler::ForAgentHost(
    DevToolsAgentHostImpl* host) {
  return DevToolsSession::HandlersForAgentHost<InputHandler>(
       host, Input::Metainfo::domainName);
 }
