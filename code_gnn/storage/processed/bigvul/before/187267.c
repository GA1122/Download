 void DevToolsAgentHostImpl::ForceDetachAllClients() {
// void DevToolsAgentHostImpl::ForceDetachAllSessions() {
    scoped_refptr<DevToolsAgentHostImpl> protect(this);
  while (!session_by_client_.empty()) {
    DevToolsAgentHostClient* client = session_by_client_.begin()->first;
    InnerDetachClient(client);
//   while (!sessions_.empty()) {
//     DevToolsAgentHostClient* client = (*sessions_.begin())->client();
//     DetachClient(client);
      client->AgentHostClosed(this);
    }
  }