void DevToolsAgentHostImpl::InnerDetachClient(DevToolsAgentHostClient* client) {
  std::unique_ptr<DevToolsSession> session =
      std::move(session_by_client_[client]);
  sessions_.erase(session.get());
  session_by_client_.erase(client);
  DetachSession(session.get());
  DevToolsManager* manager = DevToolsManager::GetInstance();
  if (manager->delegate())
    manager->delegate()->ClientDetached(this, client);
  if (sessions_.empty()) {
    io_context_.DiscardAllStreams();
    NotifyDetached();
  }
}
